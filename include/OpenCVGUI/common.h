#ifndef OPENCVGUI_COMMON_H
#define OPENCVGUI_COMMON_H


// ToDo Remove when integrate with OpenCV Contrib
#if (defined WIN32 || defined _WIN32 || defined WINCE)
# define CV_OGUI_EXPORTS __declspec(dllexport)
#else
# define CV_OGUI_EXPORTS __declspec(dllimport)
#endif

#endif