# OpenGL-1

**Learning to create an OpenGL Rendering Project**
# Dependencies Used
-GLFW(creating OpenGL context)  
-GLEW(Retrieving OpenGL data from GPU Drivers)  
-[stb_image library](https://github.com/nothings/stb/blob/5c205738c191bcb0abc65c4febfa9bd25ff35234/stb_image.h)  
-[glm Maths library](https://github.com/g-truc/glm/tree/1.0.1)  
-[ImGui Library](https://github.com/ocornut/imgui)

# Implemented
-Vertex Buffer  
-Vertex Shader  
-Fragment Shader  
-Index Buffer Object  
-Uniforms  
-Vertex Array  
-Renderer Pipeline  
-Basic Texture rendering  
-Transformations to Model View Projections  
-Basic Slider Gui for real-time transformations to matrix values

# Recent Changes
Abstracted Classes for Vertex Buffers and Vertex Arrays 04-02-25  
Abstract process to create a GPU Draw call for glDrawElements into Renderer class 05-02-25  
Add basic textures rendering 06-02-25  
Learn and add Orthogonal Projection through Vertex shader 08-02-25  
Implement ImGui Library into application.cpp and generate a basic Slider to transform ModelViewProjection values using sliders 11-02-25
![image](https://github.com/user-attachments/assets/5d29f7b9-23fb-4b46-841a-92a8bfc9f135)  
![image](https://github.com/user-attachments/assets/31840f6e-a2ba-4d09-9ef0-41e5a378ec63)  
Render another object at once using the same ModelViewProjection and different translations for each 13-02-25  
![image](https://github.com/user-attachments/assets/aeca4f56-1f9c-4f08-9c39-677ed4683ff9)
