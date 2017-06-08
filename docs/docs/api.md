---
id: api
title: API
permalink: docs/api.html
prev: hello-world.html
---
# namespace `OpenCVGUI` {#namespace_open_c_v_g_u_i}



## Summary

 Classes                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`OpenCVGUI::OGUI3D`](#class_open_c_v_g_u_i_1_1_o_g_u_i3_d)    | 
`class `[`OpenCVGUI::OGUI3DCamera`](#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera)    | 
`class `[`OpenCVGUI::OGUIArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_area)    | 
`class `[`OpenCVGUI::OGUIButton`](#class_open_c_v_g_u_i_1_1_o_g_u_i_button)    | 
`class `[`OpenCVGUI::OGUICheckBox`](#class_open_c_v_g_u_i_1_1_o_g_u_i_check_box)    | 
`class `[`OpenCVGUI::OGUICVWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_c_v_window)    | 
`class `[`OpenCVGUI::OGUIFile`](#class_open_c_v_g_u_i_1_1_o_g_u_i_file)    | 
`class `[`OpenCVGUI::OGUIFileBrowser`](#class_open_c_v_g_u_i_1_1_o_g_u_i_file_browser)    | 
`class `[`OpenCVGUI::OGUIFormArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_form_area)    | 
`class `[`OpenCVGUI::OGUIImage`](#class_open_c_v_g_u_i_1_1_o_g_u_i_image)    | 
`class `[`OpenCVGUI::OGUIImageArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_image_area)    | 
`class `[`OpenCVGUI::OGUILayout`](#class_open_c_v_g_u_i_1_1_o_g_u_i_layout)    | 
`class `[`OpenCVGUI::OGUIPlotArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_plot_area)    | 
`class `[`OpenCVGUI::OGUIRadioButtons`](#class_open_c_v_g_u_i_1_1_o_g_u_i_radio_buttons)    | 
`class `[`OpenCVGUI::OGUISlider`](#class_open_c_v_g_u_i_1_1_o_g_u_i_slider)    | 
`class `[`OpenCVGUI::OGUITextInput`](#class_open_c_v_g_u_i_1_1_o_g_u_i_text_input)    | 
`class `[`OpenCVGUI::OGUITitle`](#class_open_c_v_g_u_i_1_1_o_g_u_i_title)    | 
`class `[`OpenCVGUI::OGUIWidget`](#class_open_c_v_g_u_i_1_1_o_g_u_i_widget)    | 
`class `[`OpenCVGUI::OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)    | 


 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`OGUICVWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_c_v_window)` * namedWindow(const char * title,int width,int height)` | 
`public void app_run(std::function< void()> process_function)` | 
`public int init()` | 

## Members

### namedWindow
#### `public `[`OGUICVWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_c_v_window)` * namedWindow(const char * title,int width,int height)` {#namespace_open_c_v_g_u_i_1aa46250ebbe732e463a839a6b33928c47}



Create new Computer Vision Template Window with lateral form and stacked areas 
#### Parameters
* `{const` char*} title of window to create 


* `{int}` width of window, 1024 by default 


* `{int}` height of window, 768 by default 





#### Returns
{OGUICVWindow*} [OGUICVWindow](#class_open_c_v_g_u_i_1_1_o_g_u_i_c_v_window) created

### app_run
#### `public void app_run(std::function< void()> process_function)` {#namespace_open_c_v_g_u_i_1afb9d14584eff87fbb052006393d4f1fd}



Start main loop of UI and processing stuff as new thread. 
#### Parameters
* `process_function` function for processing computer vision thread.

### init
#### `public int init()` {#namespace_open_c_v_g_u_i_1a1eb96349d760c972cdaaa95c8f772d6d}





# class `OpenCVGUI::OGUI3D` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d}

```
class OpenCVGUI::OGUI3D
  : public OpenCVGUI::OGUIArea
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public GLuint vao` | 
`public GLuint vbo` | 
`public GLuint ibo` | 
`public GLuint shaderProgram` | 
`public GLuint MatrixID` | 
`public GLuint position_attribute` | 
`public glm::mat4 MVP` | 
`public Mat data` | 
`public int dataLength` | 
`public int dataLengthIBO` | 
`public `[`OGUI3DCamera`](#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera)` camera` | 
`public int lastMouseX` | 
`public int lastMouseY` | 
`public bool isBufferCreated` | 
`public virtual void draw(int x,int y,int width,int height)` | 
`public virtual void draw3d(int x,int y,int width,int height)` | 
`public virtual void updateScrollStatus(double xoffset,double yoffset)` | 
`public void replot(void * data)` | 
`public  OGUI3D(`[`OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)` * window,std::string title,void * data)` | 
`public void CreateVertexBuffer()` | 

## Members

### vao
##### `public GLuint vao` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1a684c3191ac187b83b510740e58820564}





### vbo
##### `public GLuint vbo` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1a9706f5f43277965318ca52a83f48f52d}





### ibo
##### `public GLuint ibo` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1aad3abe2d5031e7fb1ff4ac7925382152}





### shaderProgram
##### `public GLuint shaderProgram` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1acb43870142690d9c17bd6b610d6dbed8}





### MatrixID
##### `public GLuint MatrixID` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1a9f3684f59716085785db35dbdfa5418d}





### position_attribute
##### `public GLuint position_attribute` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1a235f0ad2ea4730499c8b213139d30da7}





### MVP
##### `public glm::mat4 MVP` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1a215445a006fa9ffe85aa0cc0a708c3c6}





### data
##### `public Mat data` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1a946906f9137d49c976a348fc68808ca5}





### dataLength
##### `public int dataLength` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1ad24f225c4fc68f111be37c0da361cc27}





### dataLengthIBO
##### `public int dataLengthIBO` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1a31bdda832670f524641f9d3b49d9d575}





### camera
##### `public `[`OGUI3DCamera`](#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera)` camera` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1acf6cac799fd43a1a62d25249deb9c300}





### lastMouseX
##### `public int lastMouseX` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1ae1690620528da53dbf4493e61bab4757}





### lastMouseY
##### `public int lastMouseY` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1acb26af749169b192788a685cef0c061f}





### isBufferCreated
##### `public bool isBufferCreated` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1a95ffcead06e5a50cd908387224d4504c}





### draw
##### `public virtual void draw(int x,int y,int width,int height)` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1a985f597eab43b1fa8c4c3c075d8a340a}





### draw3d
##### `public virtual void draw3d(int x,int y,int width,int height)` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1a32f40e885ba0f23fd5d2e57564614e61}





### updateScrollStatus
##### `public virtual void updateScrollStatus(double xoffset,double yoffset)` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1a2d423538127a277d83937ebf6089e27e}





### replot
##### `public void replot(void * data)` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1af839b1e6366d99edaa6daad0655ec10e}





### OGUI3D
##### `public  OGUI3D(`[`OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)` * window,std::string title,void * data)` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1a1d509608c0c74668ae9934af14f08365}





### CreateVertexBuffer
##### `public void CreateVertexBuffer()` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_1aa35c8918348bd4b9cf74a3af9d9ac876}





# class `OpenCVGUI::OGUI3DCamera` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::mat4 Projection` | 
`public glm::mat4 View` | 
`public glm::mat4 Model` | 
`public glm::mat4 MVP` | 
`public float x` | 
`public float y` | 
`public float z` | 
`public float phi` | 
`public float theta` | 
`public float radius` | 
`public  OGUI3DCamera()` | 
`public glm::mat4 mouseEvent(int dx,int dy)` | 
`public glm::mat4 getMVP()` | 
`public glm::mat4 setRadius(float r)` | 

## Members

### Projection
##### `public glm::mat4 Projection` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera_1a1c9a2694723f1fd17f0f458f80ab18be}





### View
##### `public glm::mat4 View` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera_1aab12d1ae5ac214c29bf1bbca6326bcb0}





### Model
##### `public glm::mat4 Model` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera_1a40ebf2825ea6732d8b073f6dfe88327d}





### MVP
##### `public glm::mat4 MVP` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera_1ab3b03d2907019c59516a5a5d3b91dc7e}





### x
##### `public float x` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera_1ad8ad6e653e48191500fd9aad8b7ba059}





### y
##### `public float y` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera_1a9392724af2ebf1367fe46670f9b132f3}





### z
##### `public float z` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera_1a58a5022d2c1f6684c6ba7a516210fa67}





### phi
##### `public float phi` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera_1a6650059cb68eb43bebb3929595cbf88e}





### theta
##### `public float theta` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera_1ac30d2a627f58de572e94ee377b5e0a41}





### radius
##### `public float radius` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera_1a4b89867e278bdf2b1d078db6f6f17238}





### OGUI3DCamera
##### `public  OGUI3DCamera()` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera_1a748f3020ffb94aa124ffce73457f0f8d}





### mouseEvent
##### `public glm::mat4 mouseEvent(int dx,int dy)` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera_1a2527f12a973be67baa0ce2e52fa7350e}





### getMVP
##### `public glm::mat4 getMVP()` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera_1a3bfbcc0b569d147361d830701ef54156}





### setRadius
##### `public glm::mat4 setRadius(float r)` {#class_open_c_v_g_u_i_1_1_o_g_u_i3_d_camera_1a5fd1af4687412a6408af41644ceb56aa}





# class `OpenCVGUI::OGUIArea` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int x` | 
`public int y` | 
`public int width` | 
`public int height` | 
`public int r` | 
`public int g` | 
`public int b` | 
`public std::string title` | 
`public int type` | 
`public bool isMaximized` | 
`public bool _show_title_bar` | 
`public `[`OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)` * window` | 
`public `[`OGUILayout`](#class_open_c_v_g_u_i_1_1_o_g_u_i_layout)` * layout` | 
`public  OGUIArea(`[`OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)` * window)` | 
`public virtual void draw(int x,int y,int width,int height)` | 
`public virtual void draw3d(int x,int y,int width,int height)` | 
`public virtual void updateScrollStatus(double xoffset,double yoffset)` | 
`public void drawTitle()` | 
`public bool isMouseIn()` | 

## Members

### x
##### `public int x` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1adaef4f679c601d0e6494a328b64732c7}





### y
##### `public int y` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1a126fe1b66570a09f1768092da9dd8535}





### width
##### `public int width` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1afedcfda9b63522012ca9ca394554cc18}





### height
##### `public int height` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1a1227141d1ae9f6877528490d879892e4}





### r
##### `public int r` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1a8b620b6181d4a381a0e1ccba52040777}





### g
##### `public int g` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1a2a9da61d98d59c93f961d74a5a54379b}





### b
##### `public int b` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1a1d3f768ead5ef26715efb205c267be7f}





### title
##### `public std::string title` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1a045aa1c27edc32f38ce5259ff47c0fab}





### type
##### `public int type` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1a5e87caff3188287e7cb420b2f1de2db8}





### isMaximized
##### `public bool isMaximized` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1af8bbea07de11aeff2ec69e6264986db0}





### _show_title_bar
##### `public bool _show_title_bar` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1a1195fe440157529495c195792ad1aa8f}





### window
##### `public `[`OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)` * window` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1ab2e8a8647a11e1f85e98d5ba1f294614}





### layout
##### `public `[`OGUILayout`](#class_open_c_v_g_u_i_1_1_o_g_u_i_layout)` * layout` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1ae3ce04546479b2f896655c194b4da5d0}



-type [OGUILayout](#class_open_c_v_g_u_i_1_1_o_g_u_i_layout)

### OGUIArea
##### `public  OGUIArea(`[`OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)` * window)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1a3a9a2ed526f65041e216713d4d7752a1}





### draw
##### `public virtual void draw(int x,int y,int width,int height)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1a61f4f34c7ddfdde0d8e40b66da5609a0}





### draw3d
##### `public virtual void draw3d(int x,int y,int width,int height)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1a0a0ed3ac3a0233d3161995a0c4330388}





### updateScrollStatus
##### `public virtual void updateScrollStatus(double xoffset,double yoffset)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1ae42a4e9373771681b45fd5732749499c}





### drawTitle
##### `public void drawTitle()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1a15a0e9acca5fce3e479f64b507c4126f}





### isMouseIn
##### `public bool isMouseIn()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_area_1a2573a94cc51828419ef792c626e0e91f}





# class `OpenCVGUI::OGUIButton` {#class_open_c_v_g_u_i_1_1_o_g_u_i_button}

```
class OpenCVGUI::OGUIButton
  : public OpenCVGUI::OGUIWidget
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public virtual void draw(int x,int y,int width)` | 
`public  OGUIButton(const char * title)` | 
`public void setCallBack(std::function< void()> func)` | 
`public void changeTitle(const char * title)` | 

## Members

### draw
##### `public virtual void draw(int x,int y,int width)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_button_1a70d4f2472ecbcd32a60ff01780dc2122}





### OGUIButton
##### `public  OGUIButton(const char * title)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_button_1aede91c5bb7de0d758f472da8383303f0}





### setCallBack
##### `public void setCallBack(std::function< void()> func)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_button_1abb73994de198636e5ae95b35b41058eb}





### changeTitle
##### `public void changeTitle(const char * title)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_button_1aa26eb27ad80e99765ee6356371de41d4}





# class `OpenCVGUI::OGUICheckBox` {#class_open_c_v_g_u_i_1_1_o_g_u_i_check_box}

```
class OpenCVGUI::OGUICheckBox
  : public OpenCVGUI::OGUIWidget
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public virtual void draw()` | 
`public  OGUICheckBox()` | 

## Members

### draw
##### `public virtual void draw()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_check_box_1ad682e778642e08b4b8ff2b40256b4038}





### OGUICheckBox
##### `public  OGUICheckBox()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_check_box_1a3300e7ab2ebd2a4e1978443a380acb1a}





# class `OpenCVGUI::OGUICVWindow` {#class_open_c_v_g_u_i_1_1_o_g_u_i_c_v_window}

```
class OpenCVGUI::OGUICVWindow
  : public OpenCVGUI::OGUIWindow
```  



Comptuer Vision template window

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< `[`OGUIArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_area)` * > areas_showing` | 
`public std::vector< `[`OGUILayout`](#class_open_c_v_g_u_i_1_1_o_g_u_i_layout)` * > layout` | 
`public `[`OGUIFormArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_form_area)` * formArea` | 
`public  OGUICVWindow(int width,int height,const char * title,int layoutOrientation)` | Create new Computer Vision Template Window with lateral form and stacked areas.
`public void imshow(string area_title,void * img)` | 
`public bool setImShowMouseClickCallBack(string area_title,std::function< void(int, int)> func)` | 
`public `[`OGUIArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_area)` * findAreaByTitle(string title)` | 
`public void addFormWidget(`[`OGUIWidget`](#class_open_c_v_g_u_i_1_1_o_g_u_i_widget)` * widget)` | 
`public void plot3D(string area_title,void * data)` | 
`public void plot2D(string area_title,void * data,std::vector< string > labels,float xstep,int plot_type)` | 

## Members

### areas_showing
##### `public std::vector< `[`OGUIArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_area)` * > areas_showing` {#class_open_c_v_g_u_i_1_1_o_g_u_i_c_v_window_1a5a1bdd606aa57c91a194c48c816f5d05}





### layout
##### `public std::vector< `[`OGUILayout`](#class_open_c_v_g_u_i_1_1_o_g_u_i_layout)` * > layout` {#class_open_c_v_g_u_i_1_1_o_g_u_i_c_v_window_1a8c9946ad2741d3de75b640dc2a89d679}





### formArea
##### `public `[`OGUIFormArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_form_area)` * formArea` {#class_open_c_v_g_u_i_1_1_o_g_u_i_c_v_window_1a3d5dba640e4552ee1f1cd64bf8f24365}





### OGUICVWindow
##### `public  OGUICVWindow(int width,int height,const char * title,int layoutOrientation)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_c_v_window_1aa6449466c9262d6a25c1c8b7410846dc}

Create new Computer Vision Template Window with lateral form and stacked areas.

This class allow create easy computer vision purposes windows where each new area created is stacked. There are similar functions than OpenCV to show images (imshow) and more missing areas for plotting and forms.


#### Parameters
* `{const` char*} title of window to create 


* `{int}` width of window, 1024 by default 


* `{int}` height of window, 768 by default 





#### Returns
{OGUICVWindow*} [OGUICVWindow](#class_open_c_v_g_u_i_1_1_o_g_u_i_c_v_window) created

### imshow
##### `public void imshow(string area_title,void * img)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_c_v_window_1aebe665258a951aae4c503911c4cd5523}



Add new image to window with title 
#### Parameters
* `{string}` area_title image are title 


* `{Mat}` image opencv mat

### setImShowMouseClickCallBack
##### `public bool setImShowMouseClickCallBack(string area_title,std::function< void(int, int)> func)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_c_v_window_1a67fb4f02c9c69b9e9135072d0277c905}



Add callback for mouse interaction to specific image area 
#### Parameters
* `{string}` area title identifier 


* `{function(int` `int)}` callback function 





#### Returns
{bool} true if setup correctly

### findAreaByTitle
##### `public `[`OGUIArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_area)` * findAreaByTitle(string title)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_c_v_window_1ac6ca583cfd4a28eb47083953c788b4d1}



Find an area by a specific title 
#### Parameters
* `{string}` title of area to search 





#### Returns
{OGUIArea*} pointer to area that is found. Null in other case

### addFormWidget
##### `public void addFormWidget(`[`OGUIWidget`](#class_open_c_v_g_u_i_1_1_o_g_u_i_widget)` * widget)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_c_v_window_1a9b30297cc5a8cbbd6fa346ce19dad4bd}





### plot3D
##### `public void plot3D(string area_title,void * data)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_c_v_window_1af1a07fc606b1453b217dd7cd9b382e65}





### plot2D
##### `public void plot2D(string area_title,void * data,std::vector< string > labels,float xstep,int plot_type)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_c_v_window_1a03beefda266cb54174d550ee6da14522}





# class `OpenCVGUI::OGUIFile` {#class_open_c_v_g_u_i_1_1_o_g_u_i_file}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public string _file_name` | 
`public string _path` | 
`public string _ext` | 
`public int _is_dir` | 
`public double _size` | 
`public void * _window` | 
`public  OGUIFile(void * window,const char * file_name,const char * path,int isdir,const char * ext,double size)` | 
`public bool draw(void * vvg,int x,int y,int mouse_x,int mouse_y)` | 
`public bool isImage()` | 
`public unsigned char * getPreview()` | 

## Members

### _file_name
##### `public string _file_name` {#class_open_c_v_g_u_i_1_1_o_g_u_i_file_1adfb04538784610abe7c9fa37aae48b04}





### _path
##### `public string _path` {#class_open_c_v_g_u_i_1_1_o_g_u_i_file_1a68446631ff70c92578b56311cf7fb3d9}





### _ext
##### `public string _ext` {#class_open_c_v_g_u_i_1_1_o_g_u_i_file_1a52026621e3967683a9a2a12342138407}





### _is_dir
##### `public int _is_dir` {#class_open_c_v_g_u_i_1_1_o_g_u_i_file_1a7178fec9350a9d07f8eec8a672b0d448}





### _size
##### `public double _size` {#class_open_c_v_g_u_i_1_1_o_g_u_i_file_1a6203b91cd2cfe109276a4b82402daa4a}





### _window
##### `public void * _window` {#class_open_c_v_g_u_i_1_1_o_g_u_i_file_1a76a172317773b97434eae21ca0160ce9}





### OGUIFile
##### `public  OGUIFile(void * window,const char * file_name,const char * path,int isdir,const char * ext,double size)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_file_1a46a3ffa218aee9572c3f93c78f6831e3}





### draw
##### `public bool draw(void * vvg,int x,int y,int mouse_x,int mouse_y)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_file_1a917e450a6c7bc7114a84679db2ff0b7f}





### isImage
##### `public bool isImage()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_file_1af467dbeda1baafef01f7e3da1804b218}





### getPreview
##### `public unsigned char * getPreview()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_file_1a749637f44f38e9b99f3aafe3239b4547}





# class `OpenCVGUI::OGUIFileBrowser` {#class_open_c_v_g_u_i_1_1_o_g_u_i_file_browser}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  OGUIFileBrowser(`[`OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)` * window)` | 
`public string open(string path,std::vector< string > filter)` | 
`public void draw()` | 

## Members

### OGUIFileBrowser
##### `public  OGUIFileBrowser(`[`OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)` * window)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_file_browser_1a35e659f9f6554f697f798ef93d1a1f2d}





### open
##### `public string open(string path,std::vector< string > filter)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_file_browser_1aa8ef34765a2228238da466ce574b36ce}





### draw
##### `public void draw()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_file_browser_1acedffb5b5a8a6adf4e77ff2ca82b104a}





# class `OpenCVGUI::OGUIFormArea` {#class_open_c_v_g_u_i_1_1_o_g_u_i_form_area}

```
class OpenCVGUI::OGUIFormArea
  : public OpenCVGUI::OGUIArea
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< `[`OGUIWidget`](#class_open_c_v_g_u_i_1_1_o_g_u_i_widget)` * > widgets` | 
`public virtual void draw(int x,int y,int width,int height)` | 
`public  OGUIFormArea(`[`OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)` * window)` | 
`public void addWidget(`[`OGUIWidget`](#class_open_c_v_g_u_i_1_1_o_g_u_i_widget)` * widget)` | 

## Members

### widgets
##### `public std::vector< `[`OGUIWidget`](#class_open_c_v_g_u_i_1_1_o_g_u_i_widget)` * > widgets` {#class_open_c_v_g_u_i_1_1_o_g_u_i_form_area_1ab981b70216da79b43d866db5697acfc4}



-type [OGUIWidget](#class_open_c_v_g_u_i_1_1_o_g_u_i_widget)

### draw
##### `public virtual void draw(int x,int y,int width,int height)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_form_area_1a2db415b6a8702fd2dafbf6e388139ed8}





### OGUIFormArea
##### `public  OGUIFormArea(`[`OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)` * window)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_form_area_1a6d3299d197e94efdfb6206fece59068d}





### addWidget
##### `public void addWidget(`[`OGUIWidget`](#class_open_c_v_g_u_i_1_1_o_g_u_i_widget)` * widget)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_form_area_1a86e33a01f9db161d821938f08f7ce763}





# class `OpenCVGUI::OGUIImage` {#class_open_c_v_g_u_i_1_1_o_g_u_i_image}

```
class OpenCVGUI::OGUIImage
  : public OpenCVGUI::OGUIWidget
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public virtual void draw()` | 
`public  OGUIImage()` | 

## Members

### draw
##### `public virtual void draw()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_image_1a430c5036aa3b32803222d6ba8dd27e9d}





### OGUIImage
##### `public  OGUIImage()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_image_1a678b63104927b2ee4d4e93cc125fd036}





# class `OpenCVGUI::OGUIImageArea` {#class_open_c_v_g_u_i_1_1_o_g_u_i_image_area}

```
class OpenCVGUI::OGUIImageArea
  : public OpenCVGUI::OGUIArea
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public virtual void draw(int x,int y,int width,int height)` | 
`public virtual void updateScrollStatus(double xoffset,double yoffset)` | 
`public  OGUIImageArea(`[`OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)` * window,string title)` | 
`public void setImage(Mat * img)` | 
`public void setMouseClickCallBack(std::function< void(int, int)> func)` | 
`public void calcHistogram()` | 

## Members

### draw
##### `public virtual void draw(int x,int y,int width,int height)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_image_area_1a5af155c8e6e2bf7f55245fda1ff25c20}





### updateScrollStatus
##### `public virtual void updateScrollStatus(double xoffset,double yoffset)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_image_area_1aa4d9dadd83bc9efa207098e31c8fb0c9}





### OGUIImageArea
##### `public  OGUIImageArea(`[`OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)` * window,string title)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_image_area_1a9f518e7c892f984fbc225773885baec9}





### setImage
##### `public void setImage(Mat * img)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_image_area_1ab744e6daefb4be5228a5961fa1f8ed99}





### setMouseClickCallBack
##### `public void setMouseClickCallBack(std::function< void(int, int)> func)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_image_area_1ab84445576177088de0d98a111eedc337}





### calcHistogram
##### `public void calcHistogram()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_image_area_1a9f86c44b9fb746a6146495300153b138}





# class `OpenCVGUI::OGUILayout` {#class_open_c_v_g_u_i_1_1_o_g_u_i_layout}

```
class OpenCVGUI::OGUILayout
  : public OpenCVGUI::OGUIArea
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< `[`OGUIArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_area)` * > areas` | 
`public virtual void draw(int x,int y,int width,int height)` | 
`public virtual void draw3d(int x,int y,int width,int height)` | 
`public virtual void updateScrollStatus(double xoffset,double yoffset)` | 
`public  OGUILayout(`[`OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)` * window,int orientation)` | 
`public void addArea(`[`OGUIArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_area)` * area)` | 
`public void setLayoutSizes(std::vector< float > sizes)` | 

## Members

### areas
##### `public std::vector< `[`OGUIArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_area)` * > areas` {#class_open_c_v_g_u_i_1_1_o_g_u_i_layout_1a30cddcab1a21a4ed05e5158316122928}



-type [OGUIArea](#class_open_c_v_g_u_i_1_1_o_g_u_i_area)

### draw
##### `public virtual void draw(int x,int y,int width,int height)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_layout_1af8c8651ee984fc7de1c29a0503236d56}





### draw3d
##### `public virtual void draw3d(int x,int y,int width,int height)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_layout_1a0067b3655612237e8db72fa7136a8391}





### updateScrollStatus
##### `public virtual void updateScrollStatus(double xoffset,double yoffset)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_layout_1a6cdf5aea6ab95e552b4baa6e47704f08}





### OGUILayout
##### `public  OGUILayout(`[`OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)` * window,int orientation)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_layout_1abeb8443bffc30dd7fa1659f4e5ea9ac7}





### addArea
##### `public void addArea(`[`OGUIArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_area)` * area)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_layout_1aeb3402642674c34be8da09ac04a38a54}





### setLayoutSizes
##### `public void setLayoutSizes(std::vector< float > sizes)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_layout_1aaf0f9bda7b9889843818bb65ab5e1b61}





# class `OpenCVGUI::OGUIPlotArea` {#class_open_c_v_g_u_i_1_1_o_g_u_i_plot_area}

```
class OpenCVGUI::OGUIPlotArea
  : public OpenCVGUI::OGUIArea
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< int > color_scheme` | 
`public virtual void draw(int x,int y,int width,int height)` | 
`public  OGUIPlotArea(`[`OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)` * window,std::string title,void * data,std::vector< std::string > labels,float xstep,int plot_type)` | 
`public void replot(void * data,std::vector< std::string > labels,float xstep)` | 
`public void setYScale(bool auto_scale,double min_value,double max_value)` | 

## Members

### color_scheme
##### `public std::vector< int > color_scheme` {#class_open_c_v_g_u_i_1_1_o_g_u_i_plot_area_1a71425aca23ec5ed99802648b7943762c}





### draw
##### `public virtual void draw(int x,int y,int width,int height)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_plot_area_1a233d004c5d6b2732030bedf578ad5d06}





### OGUIPlotArea
##### `public  OGUIPlotArea(`[`OGUIWindow`](#class_open_c_v_g_u_i_1_1_o_g_u_i_window)` * window,std::string title,void * data,std::vector< std::string > labels,float xstep,int plot_type)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_plot_area_1ac30231d13ff4f3f18936aaa3f044fdf6}





### replot
##### `public void replot(void * data,std::vector< std::string > labels,float xstep)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_plot_area_1aaf65308f2013089527294acf1778f678}





### setYScale
##### `public void setYScale(bool auto_scale,double min_value,double max_value)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_plot_area_1a9fb36d85d851e28697587073aadde9c7}





# class `OpenCVGUI::OGUIRadioButtons` {#class_open_c_v_g_u_i_1_1_o_g_u_i_radio_buttons}

```
class OpenCVGUI::OGUIRadioButtons
  : public OpenCVGUI::OGUIWidget
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public virtual void draw(int x,int y,int width)` | 
`public  OGUIRadioButtons(const char * title,std::vector< std::string > options,int default_selected)` | 
`public void setCallBack(std::function< void(int)> func)` | 

## Members

### draw
##### `public virtual void draw(int x,int y,int width)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_radio_buttons_1a4536fa28d98e863cd718df62dab92165}





### OGUIRadioButtons
##### `public  OGUIRadioButtons(const char * title,std::vector< std::string > options,int default_selected)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_radio_buttons_1a0388db6dd3773e26f20c34453d371ae9}





### setCallBack
##### `public void setCallBack(std::function< void(int)> func)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_radio_buttons_1a5df626b5296f98c999e0aae890e28731}





# class `OpenCVGUI::OGUISlider` {#class_open_c_v_g_u_i_1_1_o_g_u_i_slider}

```
class OpenCVGUI::OGUISlider
  : public OpenCVGUI::OGUIWidget
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public virtual void draw(int x,int y,int width)` | 
`public  OGUISlider(const char * title,double min,double max,double value)` | 
`public void setCallBack(std::function< void(double)> func)` | 
`public void setValue(double v)` | 

## Members

### draw
##### `public virtual void draw(int x,int y,int width)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_slider_1ae356017c6225e7696199aded9349c8bb}





### OGUISlider
##### `public  OGUISlider(const char * title,double min,double max,double value)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_slider_1a969b28218d2d76f02542f2721c1686bc}





### setCallBack
##### `public void setCallBack(std::function< void(double)> func)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_slider_1a00f2542b2c559f19e232b0eb68a7f33d}





### setValue
##### `public void setValue(double v)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_slider_1a79c4b9343dfdad94e46364e4636cf982}





# class `OpenCVGUI::OGUITextInput` {#class_open_c_v_g_u_i_1_1_o_g_u_i_text_input}

```
class OpenCVGUI::OGUITextInput
  : public OpenCVGUI::OGUIWidget
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public string value` | 
`public bool is_focus_` | 
`public int animation_alpha_` | 
`public const char * label` | 
`public virtual void draw(int x,int y,int w)` | 
`public virtual void characterCallback(unsigned int key)` | 
`public virtual void keyCallback(int key,int scancode,int action,int mods)` | 
`public  OGUITextInput(const char * label)` | 

## Members

### value
##### `public string value` {#class_open_c_v_g_u_i_1_1_o_g_u_i_text_input_1adeef1d12026be1e1fa79a648c03716dd}





### is_focus_
##### `public bool is_focus_` {#class_open_c_v_g_u_i_1_1_o_g_u_i_text_input_1a21cda208b9e1d6c1e67e11e1766c48bb}





### animation_alpha_
##### `public int animation_alpha_` {#class_open_c_v_g_u_i_1_1_o_g_u_i_text_input_1a8f67b3dca6a1edb604001edc0f409e9d}





### label
##### `public const char * label` {#class_open_c_v_g_u_i_1_1_o_g_u_i_text_input_1a1c3f824640f3d48181870b37a78f225c}





### draw
##### `public virtual void draw(int x,int y,int w)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_text_input_1aceb51b4d75f25d1e1abc538c5b987089}





### characterCallback
##### `public virtual void characterCallback(unsigned int key)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_text_input_1a342443dab4d7b6e1c70a646d6636f6c6}





### keyCallback
##### `public virtual void keyCallback(int key,int scancode,int action,int mods)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_text_input_1a6bfc6a025d45fe90c6a10e6b7b483a46}





### OGUITextInput
##### `public  OGUITextInput(const char * label)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_text_input_1a6ee653b5d13878ee300aaa64b838f24e}





# class `OpenCVGUI::OGUITitle` {#class_open_c_v_g_u_i_1_1_o_g_u_i_title}

```
class OpenCVGUI::OGUITitle
  : public OpenCVGUI::OGUIWidget
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public virtual void draw()` | 
`public  OGUITitle()` | 

## Members

### draw
##### `public virtual void draw()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_title_1a45a33dd8b684bde376c306c1b527be8d}





### OGUITitle
##### `public  OGUITitle()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_title_1a416e919ed0d782077a5cc7ed05298c71}





# class `OpenCVGUI::OGUIWidget` {#class_open_c_v_g_u_i_1_1_o_g_u_i_widget}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`OGUIFormArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_form_area)` * area` | 
`public int height` | 
`public virtual void draw(int x,int y,int widht)` | 
`public virtual void characterCallback(unsigned int key)` | 
`public virtual void keyCallback(int key,int scancode,int action,int mods)` | 

## Members

### area
##### `public `[`OGUIFormArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_form_area)` * area` {#class_open_c_v_g_u_i_1_1_o_g_u_i_widget_1a512c03b1d7e0b5e76994e4813e5889a8}





### height
##### `public int height` {#class_open_c_v_g_u_i_1_1_o_g_u_i_widget_1a760813db5c3d019304864cbdb0a633e8}





### draw
##### `public virtual void draw(int x,int y,int widht)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_widget_1a458edbfdbc2988d11a05c95f74500bc4}





### characterCallback
##### `public virtual void characterCallback(unsigned int key)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_widget_1a699ba9782fd6f000e1052bc5932b64b3}





### keyCallback
##### `public virtual void keyCallback(int key,int scancode,int action,int mods)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_widget_1ab16cb0d77ef1997915118fea32d698fa}





# class `OpenCVGUI::OGUIWindow` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * glfw_window` | 
`public void * vg` | 
`public double mouse_x` | 
`public double mouse_y` | 
`public int mouse_state` | 
`public int key_pressed` | 
`public int actual_cursor_type` | 
`public int mouse_left_state` | 
`public std::vector< void * > mouse_cursors_` | 
`public  OGUIWindow(int width,int height,const char * title,int layoutOrientation,bool fullScreen)` | 
`public  ~OGUIWindow()` | 
`public void draw()` | 
`public bool update()` | 
`public int init(bool fullScreen)` | 
`public void addArea(`[`OGUIArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_area)` * area)` | 
`public void setCursor(int cursor_type)` | 
`public void drawCursor()` | 
`public `[`OGUILayout`](#class_open_c_v_g_u_i_1_1_o_g_u_i_layout)` * getMainLayout()` | 
`public void setKeyFocus(`[`OGUIWidget`](#class_open_c_v_g_u_i_1_1_o_g_u_i_widget)` * widget)` | 
`public `[`OGUIWidget`](#class_open_c_v_g_u_i_1_1_o_g_u_i_widget)` * getKeyFocusWidget()` | 
`public int getWindowHeight()` | 
`public int getWindowWidth()` | 
`public void updatePerfGraph()` | 
`public void showPerfGraph(bool show)` | 
`public void setExternal2DDraw(std::function< void(void *context)> func)` | 
`public int popup(string title,string text,int type)` | Popups.
`public string openFileBrowser(string path,std::vector< string > filter)` | File browser.
`public void close()` | 
`public int getStatus()` | 
`public void maximizeArea(`[`OGUIArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_area)` * area)` | 

## Members

### glfw_window
##### `public void * glfw_window` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a56d181d083f35e7eae72dee17cd16d92}





### vg
##### `public void * vg` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1ad73972cd21ecd7da37ce9ec6260195cb}





### mouse_x
##### `public double mouse_x` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1af40d5267e143263b76fa6348805adc03}





### mouse_y
##### `public double mouse_y` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1acb315b67254cf4d56c5f96f26f4034c4}





### mouse_state
##### `public int mouse_state` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a19f481b356576803212c3e6a1aab4d78}





### key_pressed
##### `public int key_pressed` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a883326e6fc4a7a2cb7f5e9056bf2b133}





### actual_cursor_type
##### `public int actual_cursor_type` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a5483ab4149297446d12b3f7db51730b4}





### mouse_left_state
##### `public int mouse_left_state` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a07540983006556c35ecd8dd2dec6c896}





### mouse_cursors_
##### `public std::vector< void * > mouse_cursors_` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1ac732dcb731d32fa8c00d21bf2ce27265}





### OGUIWindow
##### `public  OGUIWindow(int width,int height,const char * title,int layoutOrientation,bool fullScreen)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a7eedb16af7e06c8c3ff174c5f31a9c76}





### ~OGUIWindow
##### `public  ~OGUIWindow()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a33bc14b906fc61ffeb15e1723e521373}





### draw
##### `public void draw()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a54baa6fd799bf44514081e953ea3d233}





### update
##### `public bool update()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a8258eb453ed57d610771df1b7585ff81}





### init
##### `public int init(bool fullScreen)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1af1e737bea59407b3b0e4f11910404824}





### addArea
##### `public void addArea(`[`OGUIArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_area)` * area)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1ae521f6749a272dd4461e6018bee9eabf}





### setCursor
##### `public void setCursor(int cursor_type)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1ab198c9aa4125e674044080f370c7919b}





### drawCursor
##### `public void drawCursor()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a637a44f2b96d50c660c30147651372da}





### getMainLayout
##### `public `[`OGUILayout`](#class_open_c_v_g_u_i_1_1_o_g_u_i_layout)` * getMainLayout()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a0e6a0940955b4659508c5bba2e86d972}





### setKeyFocus
##### `public void setKeyFocus(`[`OGUIWidget`](#class_open_c_v_g_u_i_1_1_o_g_u_i_widget)` * widget)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a6c19b9d40550edc84d492a10833880a9}





### getKeyFocusWidget
##### `public `[`OGUIWidget`](#class_open_c_v_g_u_i_1_1_o_g_u_i_widget)` * getKeyFocusWidget()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1acbf16bf271046d6fe1a4454b77b6fa84}





### getWindowHeight
##### `public int getWindowHeight()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a5daac852ade6b652e096583fe82be372}





### getWindowWidth
##### `public int getWindowWidth()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a468c04c92e49a8197b21d752b5b5974b}





### updatePerfGraph
##### `public void updatePerfGraph()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a5b4d69f79390463a132f19a3e1ee6313}





### showPerfGraph
##### `public void showPerfGraph(bool show)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a44f4bf01265937c1ed101a889fd91b61}





### setExternal2DDraw
##### `public void setExternal2DDraw(std::function< void(void *context)> func)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a4a208f57ee6db33d9e21ce340063cff3}





### popup
##### `public int popup(string title,string text,int type)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a7bad15ac09b4456cb09880a9e151ae7b}

Popups.



### openFileBrowser
##### `public string openFileBrowser(string path,std::vector< string > filter)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a9d5976dd1eb4642fd36d171cf70fe5bc}

File browser.



### close
##### `public void close()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a833428df1125ccc8c0254ba0c95e98f3}





### getStatus
##### `public int getStatus()` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a2a1b55fe67976386d6cbd96fe4894653}





### maximizeArea
##### `public void maximizeArea(`[`OGUIArea`](#class_open_c_v_g_u_i_1_1_o_g_u_i_area)` * area)` {#class_open_c_v_g_u_i_1_1_o_g_u_i_window_1a39e2d6dd86bd32da69d6fd4e18abd06e}





# struct `GPUtimer` {#struct_g_p_utimer}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int supported` | 
`public int cur` | 
`public int ret` | 
`public unsigned int queries` | 

## Members

### supported
##### `public int supported` {#struct_g_p_utimer_1ab54d9f4fe1fc810f9e8d736bf9381826}





### cur
##### `public int cur` {#struct_g_p_utimer_1ac75c6fd1ca2bbd22f4f3155f06a2fffb}





### ret
##### `public int ret` {#struct_g_p_utimer_1aa28be2ddd179ec58bb62561770ee361c}





### queries
##### `public unsigned int queries` {#struct_g_p_utimer_1a19f7265e63c7f0ca32dcc432e975e88c}





# struct `PerfGraph` {#struct_perf_graph}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int style` | 
`public char name` | 
`public float values` | 
`public int head` | 

## Members

### style
##### `public int style` {#struct_perf_graph_1a0aa8f294ed1eee979ded828d3208b39e}





### name
##### `public char name` {#struct_perf_graph_1a8abe0c9b9f33916090cc186c489bd4f0}





### values
##### `public float values` {#struct_perf_graph_1a98e2a254e5a66226a5fac6a5c26eea7c}





### head
##### `public int head` {#struct_perf_graph_1ac2e6534450ce170d33608a56f07c1794}





