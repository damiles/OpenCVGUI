#ifndef OpenCVGUI_OGUILayout_h
#define OpenCVGUI_OGUILayout_h

#include <vector>

#include "OGUIArea.h"
namespace OpenCVGUI {
    class OGUIWindow;
}

namespace OpenCVGUI {    
class OGUILayout : public OGUIArea {

 public:

    void draw(int x, int y, int width, int height);
    void draw3d(int x, int y, int width, int height);
    virtual void updateScrollStatus(double xoffset,double yoffset);
    OGUILayout(OGUIWindow* window, int orientation);
    void addArea(OGUIArea* area);
    void setLayoutSizes(std::vector<float> sizes);

 private:
    int orientation;
    bool is_pressed=false;
    int id_area_pressed=0;
    std::vector<float> area_sizes;
 public:

    /**
     * @element-type OGUIArea
     */
    std::vector< OGUIArea* > areas;
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUILayout_h
