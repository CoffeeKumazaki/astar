#include <stdafx.hpp>
#include "imgui_helper.hpp"
#include <astar.hpp>

int main(int argc, char const *argv[]) {

	int w = 1280;
	int h = 720;
	std::string title = "Astar demo";
	initImgui(w, h, title);

	std::vector<double> x = {0, 0, 20, 20, 40, 60, 60 };
	std::vector<double> y = {0, 20, 10, 50, 30, 0, 50 };

	std::vector<NODE_PTR> nodes;

	for (size_t i = 0; i < x.size(); i++)	{
		NODE_PTR n = std::make_shared<Node>();
		n->id = i;
		n->x = x[i];
		n->y = y[i];
		nodes.push_back(n);
	}

	nodes[0]->child.push_back(std::make_pair(nodes[1], nodes[0]->estimate_cost(nodes[1])));
	nodes[0]->child.push_back(std::make_pair(nodes[2], nodes[0]->estimate_cost(nodes[2])));
	nodes[1]->child.push_back(std::make_pair(nodes[0], nodes[1]->estimate_cost(nodes[0])));
	nodes[1]->child.push_back(std::make_pair(nodes[2], nodes[1]->estimate_cost(nodes[2])));
	nodes[1]->child.push_back(std::make_pair(nodes[3], nodes[1]->estimate_cost(nodes[3])));
	nodes[2]->child.push_back(std::make_pair(nodes[0], nodes[2]->estimate_cost(nodes[0])));
	nodes[2]->child.push_back(std::make_pair(nodes[4], nodes[2]->estimate_cost(nodes[4])));
	nodes[2]->child.push_back(std::make_pair(nodes[1], nodes[2]->estimate_cost(nodes[1])));
	nodes[3]->child.push_back(std::make_pair(nodes[1], nodes[3]->estimate_cost(nodes[1])));
	nodes[3]->child.push_back(std::make_pair(nodes[6], nodes[3]->estimate_cost(nodes[6])));
	nodes[4]->child.push_back(std::make_pair(nodes[2], nodes[4]->estimate_cost(nodes[2])));
	nodes[4]->child.push_back(std::make_pair(nodes[5], nodes[4]->estimate_cost(nodes[5])));
	nodes[4]->child.push_back(std::make_pair(nodes[6], nodes[4]->estimate_cost(nodes[6])));
	nodes[5]->child.push_back(std::make_pair(nodes[4], nodes[5]->estimate_cost(nodes[4])));
	nodes[6]->child.push_back(std::make_pair(nodes[3], nodes[6]->estimate_cost(nodes[3])));
	nodes[6]->child.push_back(std::make_pair(nodes[5], nodes[6]->estimate_cost(nodes[5])));

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec4 node_color = ImVec4(230.0/255.0, 180.0/255.0f, 34.0f/255.0, 1.00f);
	ImVec4 edge_color = ImVec4(255.0/255.0, 255.0/255.0f, 255.0f/255.0, 1.00f);

	Astar astar;
	int sid = 0;
	int gid = 5;
	astar.setStartGoal(nodes[sid], nodes[gid]);

	std::list<NODE_PTR> lpath;
	astar.findPath(lpath);
	std::vector<NODE_PTR> path(lpath.begin(), lpath.end());

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_NoCollapse;
			window_flags |= ImGuiWindowFlags_NoBackground;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_FirstUseEver);

			ImGui::Begin("Astar", nullptr, window_flags);
			ImDrawList *draw_list = ImGui::GetWindowDrawList();

			for (size_t i = 0; i < nodes.size(); i++) {
				for (size_t j = 0; j < nodes[i]->child.size(); j++) {
					draw_list->AddLine(
						ImVec2(nodes[i]->child[j].first->x*5 + 50, nodes[i]->child[j].first->y*3 + 100), 
						ImVec2(nodes[i]->x*5 + 50, nodes[i]->y*3 + 100), ImColor(edge_color));
				}

				if (i == sid) {
					draw_list->AddCircleFilled(ImVec2(nodes[i]->x * 5 + 50, nodes[i]->y * 3 + 100), 10, ImColor(ImVec4(1.0,0.0,0.0,1.0)));
				}
				else if (i == gid) {
					draw_list->AddCircleFilled(ImVec2(nodes[i]->x * 5 + 50, nodes[i]->y * 3 + 100), 10, ImColor(ImVec4(0.0,0.0,1.0,1.0)));
				}
				else {
					draw_list->AddCircle(ImVec2(nodes[i]->x*5 + 50, nodes[i]->y*3 + 100), 10, ImColor(node_color), 0, 1.5);
				}
			}

			for (size_t i = 0; i < path.size(); i++) {
				draw_list->AddCircle(ImVec2(path[i]->x*5 + 50, path[i]->y*3 + 100), 10, ImColor(ImVec4(0.0,0.0,0.0,1.0)), 0, 1.5);
			}

			ImGui::Text("Cost %lf", nodes[gid]->f);

			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	termImgui();
	return 0;
}
