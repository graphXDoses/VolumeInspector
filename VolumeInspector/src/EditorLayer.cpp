#include "EditorLayer.h"
#include "Engine/Platform/PlatformUtils.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

//#include "Engine/Graphics/Primitives/QuadMesh.h"
#include "Engine/Graphics/Primitives/CubeMesh.h"
#include "CustomWidgets/CustomWidgets.h"

namespace MobiusEngine
{

	static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 1.0f));

	EditorLayer::EditorLayer()
		: Layer("Example")
	{
		std::filesystem::current_path(m_path);
	}

	void EditorLayer::OnAttach()
	{
		//Application::Get().GetImGuiLayer()->BlockEvents(false);
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_EditorCamera = PerspectiveCamera(45.0f, 1.778f, 0.1f, 1000.0f);
		m_SquareVA = VertexArray::Create();

		Ref<VertexBuffer> squareVB = VertexBuffer::Create(Meshes::cubeVertices, sizeof(Meshes::cubeVertices));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "aPos" }
		};
		squareVB->SetLayout(layout);
		m_SquareVA->AddVertexBuffer(squareVB);

		Ref<IndexBuffer> squareIB = IndexBuffer::Create(Meshes::cubeIndices, sizeof(Meshes::cubeIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330

			uniform mat4 matProj;
			uniform mat4 matView;
			uniform mat4 matModel;
			uniform float ScaleFac;

			layout (location = 0) in vec3 aPos;

			out vec3 pos;
			out mat4 modelViewInv;

			void main() {
				gl_Position = matProj * matView * matModel * vec4(aPos*ScaleFac, 1);
				pos = aPos*ScaleFac;
				modelViewInv = inverse(matView * matModel);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330

			in vec3 pos;
			in mat4 modelViewInv;
			out vec4 O;

			uniform float opacity, clipPlaneDepth, isolateMin, isolateMax;
			uniform vec3 clipPlaneNormal;
			uniform bool colorizeIntensity, clip, flipClipPlane;
			uniform sampler2D tex;

			const float maxDist = sqrt(2.);
			const int numSamples = 256;
			const float stepSize = maxDist/float(numSamples);

			float opacityFactor = opacity,
			intensityMin = min(isolateMin, isolateMax)*.01,
			intensityMax = max(isolateMin, isolateMax)*.01;

			const float lightFactor = 1.5;
			const float numberOfSlices = 96.;
			const float slicesOverX = 10.;
			const float slicesOverY = 10.;

			struct Ray 
			{
				vec3 Origin;
				vec3 Dir;
			};

			struct AABB 
			{
				vec3 Min;
				vec3 Max;
			};

			bool IntersectBox(Ray r, AABB aabb, out float t0, out float t1)
			{
				vec3 invR = 1. / r.Dir;
				vec3 tbot = invR * (aabb.Min-r.Origin);
				vec3 ttop = invR * (aabb.Max-r.Origin);
				vec3 tmin = min(ttop, tbot);
				vec3 tmax = max(ttop, tbot);
				vec2 t = max(tmin.xx, tmin.yz);
				t0 = max(t.x, t.y);
				t = min(tmax.xx, tmax.yz);
				t1 = min(t.x, t.y);
				return t0 <= t1;
			}

			float getVoxel(sampler2D atlas, vec3 volpos)
			{
				volpos.x = 1.-volpos.x;
				float s1,s2;
				float dx1,dy1;
				float dx2,dy2;

				vec2 texpos1,texpos2;

				s1 = floor(volpos.z*numberOfSlices);
				s2 = s1+1.;

				dx1 = fract(s1/-slicesOverX);
				dy1 = floor(s1/slicesOverY)/slicesOverY;

				dx2 = fract(s2/-slicesOverX);
				dy2 = floor(s2/slicesOverY)/slicesOverY;
	
				texpos1.x = dx1+(volpos.x/-slicesOverX);
				texpos1.y = dy1+(volpos.y/slicesOverY);

				texpos2.x = dx2+(volpos.x/-slicesOverX);
				texpos2.y = dy2+(volpos.y/slicesOverY);

				return mix( textureLod(atlas,texpos1,0.).x, textureLod(atlas,texpos2,0.).x, (volpos.z*numberOfSlices)-s1);
			}

			//standard hot-to-cold color ramp
			vec3 Heatmap(float v,float vmin,float vmax)
			{
				vec3 c = vec3(1);
				float dv = 0.;
	  
				v = clamp(v, vmin, vmax);
				dv = vmax - vmin;
	
				if (v < (vmin + .25 * dv)) {
				  c.r = 0.;
				  c.g = 4. * (v - vmin) / dv;
				} else if (v < (vmin + .5 * dv)) {
				  c.r = 0.;
				  c.b = 1. + 4. * (vmin + .25 * dv - v) / dv;
				} else if (v < (vmin + .75 * dv)) {
				  c.r = 4. * (v - vmin - .5 * dv) / dv;
				  c.b = 0.;
				} else {
				  c.g = 1. + 4. * (vmin + .75 * dv - v) / dv;
				  c.b = 0.;
				}
	
				return c;
			}

			vec3 plnNorm = sign(float(flipClipPlane)-.5)*normalize(clipPlaneNormal);

			void main() {
				vec3 view = normalize(pos - modelViewInv[3].xyz);
				Ray eye = Ray( modelViewInv[3].xyz, view );
	
				AABB aabb = AABB(vec3(-1), vec3(1));
	
				float tnear, tfar;
				IntersectBox(eye, aabb, tnear, tfar);
				tnear = mix(tnear, 0., tnear < 0.);
	
				vec3 rayStart = eye.Origin + eye.Dir * tnear;
				vec3 rayStop = eye.Origin + eye.Dir * tfar;
				rayStart = .5 * (rayStart + 1.);
				rayStop = .5 * (rayStop + 1.);

				vec3 pos = rayStart;
				vec3 dir = rayStop - rayStart;
				vec3 marchDist = normalize(dir) * stepSize;
				float travel = distance(rayStop, rayStart);
	
				float len = length(dir);
				dir = normalize(dir);
	
				if(clip)
				{
					float dis = dot(dir, plnNorm);
					bool frontface = (dot(dir , plnNorm) > 0.);
					if (dis != 0.  )  dis = ((sign(float(flipClipPlane)-.5)*clipPlaneDepth)-dot(plnNorm, rayStart-.5)) / dis;
					if ((!frontface) && (dis < 0.)) return;
					if ((frontface) && (dis > len)) return;
					if ((dis > 0.) && (dis < len)) {
						if (frontface) {
							rayStart = rayStart + dir * dis;
						} else {
							rayStop =  rayStart + dir * dis; 
						}
	
						pos = rayStart;
						marchDist = normalize(rayStop-rayStart) * stepSize;
						travel = distance(rayStop, rayStart);   
					}
				}
	
				vec4 accum, vsample, value;
	
				for (int i=0; i < numSamples && travel > 0.; ++i, pos += marchDist, travel -= stepSize) {
					float fade = clamp(distance(pos,rayStop)/stepSize,0.,1.);
					float tf_pos = getVoxel(tex, pos);
		
					value.rgb = mix(vec3(tf_pos), Heatmap(tf_pos,0.,1.), float(colorizeIntensity));
		
					// Isolate intesity areas, histogram-wise.
					value.a = pow(smoothstep(intensityMax,(intensityMax+intensityMin)*0.5,tf_pos)*
						smoothstep(intensityMin,(intensityMax+intensityMin)*0.5,tf_pos),0.25);
        

					// Process the volume sample
					vsample.a = value.a * opacityFactor * (1./float(numSamples))*fade;
					vsample.rgb = value.rgb * vsample.a * lightFactor;
						
					accum.rgb += (1. - accum.a) * vsample.rgb;
					accum.a += vsample.a;

		
					if(accum.a>=1.) break;

				}

				O.rgb = accum.rgb;
				O.a = accum.a;
			}
		)";

		m_Shader = Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);
		
		NGN_TRACE("Current path: \'{0}\'", m_path.string());
		m_Texture = Texture2D::Create(128, 128);

		m_Shader->Bind();
		m_Shader->SetInt("tex", 0);

	}

	void EditorLayer::OnDetach() {}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		lat = ts.GetMilliseconds();
		m_EditorCamera.OnUpdate(ts);

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_viewportPanelSize.x, m_viewportPanelSize.y);
		}

		m_Framebuffer->Bind();

		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		RenderCommand::Clear();

		m_Framebuffer->ClearAttachment(1, -1);

		Renderer::BeginScene(m_EditorCamera);

		if(m_Texture)
			m_Texture->Bind();

		m_Shader->Bind();
		m_Shader->SetFloat("ScaleFac", m_scl);
		m_Shader->SetFloat("opacity", m_opacity);
		m_Shader->SetFloat("clipPlaneDepth", m_clipPlaneDepth);
		m_Shader->SetFloat("isolateMin", m_isolateMin);
		m_Shader->SetFloat("isolateMax", m_isolateMax);
		m_Shader->SetFloat3("clipPlaneNormal", rotatePlaneNormal(&m_lockTrack));
		m_Shader->SetInt("colorizeIntensity", m_colorizeIntensity);
		m_Shader->SetInt("clip", m_clip);
		m_Shader->SetInt("flipClipPlane", m_flipClipPlane);

		Renderer::Submit(m_Shader, m_SquareVA, BBConfig(90, glm::vec3(-1,0,0)) * scale);
		Renderer::EndScene();

		m_Framebuffer->Unbind();
	}

	glm::vec3& EditorLayer::rotatePlaneNormal(bool* lock)
	{
		if (*lock)
		{
			glm::vec4 tmp = glm::vec4(glm::normalize(m_EditorCamera.GetPosition()), 1.0f) * BBConfig(90, glm::vec3(-1, 0, 0)) *scale;

			m_clipPlaneNormal = glm::vec3(tmp.x, tmp.y, tmp.z);
			m_clipPlanePitch = -glm::degrees(m_EditorCamera.GetPitch());
			m_clipPlaneYaw = -glm::degrees(m_EditorCamera.GetYaw());
		}
		else {
			m_clipPlaneNormal = -glm::vec4(glm::normalize(PitchYawToDirection()), 1.0f) * BBConfig(90, glm::vec3(-1, 0, 0)) *scale;
		}
		return m_clipPlaneNormal;
	}

	void EditorLayer::ResetCameraOrientation()
	{
		m_EditorCamera.ResetView();
	}

	bool EditorLayer::ResetCameraOrientation(KeyPressedEvent& e)
	{
		if (e.IsRepeat())
			return false;
		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);

		switch (e.GetKeyCode())
		{
			case Key::KP0:
			{
				if (control)
					m_EditorCamera.ResetView();

				break;
			}
		}
		return false;
	}

	bool EditorLayer::SetPlaneDepth(MouseScrolledEvent& e)
	{
		if (Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl))
		{
			m_clipPlaneDepth -= e.GetYOffset() * 0.01f;
		}

		return false;
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// ImGuiWindowFlags_NoDocking flag makes the parent window not dockable into,
		// Disclaimer: It would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render background and handle the pass-thru hole,
		// so Begin() is configured to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: Even if Begin() returns false (aka window is collapsed), behavour is acceptable as it keeps DockSpace() active.
		// If a DockSpace() is inactive, all active windows docked into it will lose their parent and become undocked.
		// Docking relationship cannot be preserved between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("MainDockSpace", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Application"))
			{
				if (ImGui::BeginMenu("Atlas Texture"))
				{
					if (ImGui::MenuItem("Import"))
						ImportImage();
					
					if (!m_Texture->IsLoaded())
					{
						ImGui::BeginDisabled();
						ImGui::MenuItem("Clear Cache");
						ImGui::EndDisabled();
					}
					else {
						if (ImGui::MenuItem("Clear Cache"))
							m_clearCacheDialogOpen = true;
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
					Application::Get().Close();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Camera"))
			{
				if (ImGui::MenuItem("Reset", "Ctrl + Num0"))
					ResetCameraOrientation();

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (m_clearCacheDialogOpen)
		{
			ImGui::OpenPopup("Atlas Cache");
			ImGui::SetNextWindowSize(ImVec2(600, 90));
		}

		if (ImGui::BeginPopupModal("Atlas Cache",0,ImGuiWindowFlags_NoResize))
		{
			ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
			ImGui::Text("Texture information used from \'%s\' will cease and the reference to the file will be lost. Do you wish to proceed?", m_lastPath.c_str());
			ImGui::PopTextWrapPos();
			if (ImGui::Button("Clear"))
			{
				m_Texture = Texture2D::Create(128,128);
				m_showLegend = false;
				m_lastPath = "";
				m_clearCacheDialogOpen = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Abort"))
			{
				m_clearCacheDialogOpen = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };


		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		//Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

		ImVec2 VPS = ImGui::GetContentRegionAvail();

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), VPS, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		m_viewportPanelSize = glm::vec2(VPS.x, VPS.y);
		m_EditorCamera.SetViewportSize(m_viewportPanelSize.x, m_viewportPanelSize.y);

		if (m_showLegend)
			MobiusUI::IntensityLegend("##IntensityLegend", &m_colorizeIntensity);

		ImGui::PopStyleVar();
		if (ImGui::BeginPopupContextWindow())
		{
			ImGui::Text("Options");
			ImGui::Separator();
			if (ImGui::MenuItem("Show Ledgend", 0, m_showLegend, m_Texture->IsLoaded()))
			{
				m_showLegend = !m_showLegend;
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Display");
		if (ImGui::BeginTable("Display_Cols", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_PreciseWidths))
		{
			ImGui::TableNextRow(); ImGui::TableNextColumn();
			ImGui::Text("Opacity");
			ImGui::TableNextColumn();
			ImGui::PushItemWidth(-1);
			ImGui::SliderFloat("##opacity", &m_opacity, 0.0f, 100.0f, "%.1f%%", 1.0f);
			ImGui::PopItemWidth();
			ImGui::TableNextRow(); ImGui::TableNextColumn();
			ImGui::Text("Intensity Range");
			ImGui::TableNextColumn();
			ImGui::PushItemWidth(-1);
			ImGui::DragFloatRange2("##", &m_isolateMin, &m_isolateMax, 0.25f, 0.0f, 100.0f, "Min: %.1f%%", "Max: %.1f%%", ImGuiSliderFlags_AlwaysClamp);
			ImGui::PopItemWidth();

			ImGui::EndTable();
		}

		ImGui::Separator();

		ImGui::Text("Color Mapping:");
		MobiusUI::ColorMapSwitch(&m_colorizeIntensity);

		ImGui::End();

		ImGui::Begin("Clipping");
		if(ImGui::BeginTable("Clipping_Cols", 2, ImGuiTableFlags_BordersInnerV))
		{
			ImGui::TableNextRow(); ImGui::TableNextColumn();
			ImGui::Text("Enable Clipping");
			ImGui::TableNextColumn();
			ImGui::Checkbox("##clip", &m_clip);
			ImGui::TableNextRow(); ImGui::TableNextColumn();
			ImGui::BeginDisabled(!m_clip);
			ImGui::Text("Lock track to camera");
			ImGui::TableNextColumn();
			ImGui::Checkbox("##lockTrack", &m_lockTrack);
			ImGui::TableNextRow(); ImGui::TableNextColumn();
			ImGui::Text("Pitch");
			ImGui::TableNextColumn();
			ImGui::PushID("##pitch");
			ImGui::DragFloat("##pitch", &m_clipPlanePitch, 1.0f, -360.0f, 360.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
				m_clipPlanePitch = 0.0f;
			ImGui::PopID();
			ImGui::TableNextRow(); ImGui::TableNextColumn();
			ImGui::Text("Yaw");
			ImGui::TableNextColumn();
			ImGui::PushID("##yaw");
			ImGui::DragFloat("##yaw", &m_clipPlaneYaw, 1.0f, -360.0f, 360.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
				m_clipPlaneYaw = 0.0f;
			ImGui::PopID();
			ImGui::TableNextRow(); ImGui::TableNextColumn();
			ImGui::Text("Depth");
			ImGui::TableNextColumn();
			ImGui::PushID("##depth");
			ImGui::DragFloat("##depth", &m_clipPlaneDepth, 0.01f, -1.0f, 1.0f, "%.2f", 1.0f);
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
				m_clipPlaneDepth = 0.0f;
			ImGui::PopID();
			
			ImGui::EndTable();
		}
		ImGui::BeginColumns("flip_ColSet", 3, ImGuiOldColumnFlags_NoBorder);
		ImGui::NextColumn();
		ImGui::PushID("Flip Normal");
		if (ImGui::Button("Flip Normal", ImVec2(ImGui::GetColumnWidth(), 24.0f)))
			m_flipClipPlane = !m_flipClipPlane;
		if (ImGui::IsItemHovered())
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		ImGui::PopID();
		ImGui::EndDisabled();
		ImGui::EndColumns();
		ImGui::End();

		ImGui::Begin("Performance");
		ImGui::Text("GPU Latency: %.3f sec ( %.2f fps )", lat / 1000.0f, floorf(1000.0f / lat));
		ImGui::End();

		ImGui::End();
	}

	void EditorLayer::ImportImage()
	{
		std::string filepath = FileDialogs::OpenFile("Image Files (*.png, *.jpg)\0*.png;*.jpg\0");
		if (!filepath.empty())
			ImportImage(filepath);
	}

	void EditorLayer::ImportImage(const std::filesystem::path& path)
	{
		m_lastPath = path.string();
		m_Texture = Texture2D::Create(m_lastPath);
		m_showLegend = true;
		auto const pos = m_lastPath.find_last_of('\\');
		m_lastPath = m_lastPath.substr(pos + 1);
	}

	glm::vec3 EditorLayer::PitchYawToDirection()
	{
		return glm::rotate(glm::quat(glm::vec3(glm::radians(m_clipPlanePitch), glm::radians(m_clipPlaneYaw), 0.0f)), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_EditorCamera.OnEvent(event);
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(NGN_BIND_EVENT_FNC(EditorLayer::ResetCameraOrientation));
		dispatcher.Dispatch<MouseScrolledEvent>(NGN_BIND_EVENT_FNC(EditorLayer::SetPlaneDepth));
		NGN_TRACE("{0}", event.ToString());
	}

}