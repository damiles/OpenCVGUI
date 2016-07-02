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
    OGUILayout(OGUIWindow* window, int orientation);
    void addArea(OGUIArea* area);

 private:
    int orientation;
    
 public:

    /**
     * @element-type OGUIArea
     */
    std::vector< OGUIArea* > areas;
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUILayout_h
