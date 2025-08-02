# Good Grid

## <cb>The ultimate DrawGridLayer rewrite and API!</c>

- <cg>Better Performance!</c>
- <cg>Rotation Support!</c>
- <cg>Extensive API</c>
- <cg>Bug Fixes</c>
- <cg>Exactly Like Vanilla (by default)</c>

Here's what you can do with the API as a Developer:

## LineColor

This is a class that contains up to 2 colors for solid or gradient lines. When passing one into the constructor, both color A and B equal the same value. If you pass two colors, the line will be a gradient between the two. 

```cpp
LineColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
```
Constructor with rgba bytes.

```cpp
LineColor(const cocos2d::ccColor4B& colorA);
```
Constructor with one color, will make the line solid.

```cpp
LineColor(const cocos2d::ccColor4B& colorA, const cocos2d::ccColor4B& colorB);
```
Constructor with two colors, will make the line a gradient between the two colors.

```cpp
cocos2d::ccColor4B getColorA() const
```
Returns the first color.

```cpp
cocos2d::ccColor4B getColorB() const
```
Returns the second color if it is set, else returns the first color.

## DrawGridAPI.hpp

```cpp
void markDirty()
```
Marks the grid as dirty, forcing it to update it's culling bounds.

```cpp
bool isDirty()
```
Returns true if the grid is marked as dirty.

```cpp
void setVanillaDraw(bool enabled)
```
Sets the grid rendering to Vanilla's implementation (used mostly for debugging, should not be used in a mod).

```cpp
bool isVanillaDraw()
```
Returns true if vanilla draw is enabled.

```cpp
void setLineSmoothing(bool enabled)
```
**Line smoothing is only supported on Windows and MacOS!**
Sets all lines to smooth (useful for editor camera rotation)

```cpp
bool hasLineSmoothing()
```
**Line smoothing is only supported on Windows and MacOS!**
Returns true of line smoothing is enabled

```cpp
void setLineSmoothingLimit(float limit)
```
**Line smoothing is only supported on Windows and MacOS!**
Sets the editor zoom limit (if >= than this limit, the lines will not be made smooth) for line smoothing as having too many smoothed lines on screen at once is laggy. I recommend keeping it at default.

```cpp
float getLineSmoothingLimit()
```
Returns the line smoothing limit

```cpp
void overrideGridBoundsSize(cocos2d::CCSize size)
```
Sets the grid's max width and height bounds (useful for editor extenders and limiters).

```cpp
cocos2d::CCSize getGridBoundsSize()
```
Returns the max width and height bounds of the grid.

```cpp
void overrideGridBoundsOrigin(cocos2d::CCPoint origin)
```
Sets the grid's origin (bottom left) coordinates.

```cpp
cocos2d::CCPoint getGridBoundsOrigin()
```
Returns the coordinates of the grid's origin.

```cpp
cocos2d::CCSize getWorldViewSize()
```
Returns the World View size. This size is the window size scaled by the editor zoom multiplied by the overdraw factor to account for rotation.

```cpp
float getOverdrawFactor()
```
Returns the overdraw factor for the rotated editor view.

```cpp
void drawLine(const cocos2d::ccVertex2F& start, const cocos2d::ccVertex2F& end, const LineColor& color, float width, bool blend = false)
```
Draws a line to the screen with two coordinates where it starts and ends. Takes in a LineColor param which allows for a solid or gradient color. The width is the line width, for the purpose of optimization, this also determines Z ordering for that specific width. Blending is an optional param that will make the line blend with additive blending.

```cpp
void drawRect(const cocos2d::CCRect& rect, const cocos2d::ccColor4B& color, bool blend = false)
```
Draws a rectangle to the screen with a CCRect param dictating the bounds and a color param to set its color. Blending is an optional param that will make the rectangle blend with additive blending.

```cpp
void drawRectOutline(const cocos2d::CCRect& rect, const cocos2d::ccColor4B& color, float width, bool blend = false)
```
Draws a rectangle outline to the screen with a CCRect param dictating the bounds, a color param to set its color, and the width of the outline. Blending is an optional param that will make the rectangle outline blend with additive blending.

```cpp
bool isObjectVisible(GameObject* object)
```
Util to check if an object is visible (accounting for selecting, group and hide options).

```cpp
geode::Result<DrawNode&> getNodeByID(const std::string& id)
```
Returns a result of a DrawNode if one by the passed in ID exists.

```cpp
template <typename T>
requires std::is_base_of_v<DrawNode, T>
geode::Result<T&> getNode(const std::string& id)
```
Returns a result of a DrawNode if one passed by the type and ID exists.

```cpp
template <typename T>
requires std::is_base_of_v<DrawNode, T>
geode::Result<T&> getNode()
```
Returns a result of a DrawNode if one passed by the type. Will always be first of a type if multiple exist.

```cpp
template <typename T, typename... Args>
requires std::is_base_of_v<DrawNode, T>
T& addDraw(const std::string& id, Args&&... args)
```
Returns a reference of and adds a custom DrawNode to the DrawGridLayer with an ID.

## DrawNode.hpp

This class is what makes up every part of the new DrawGridLayer. Every layer on it inherits this class. It provides a few methods as well as some virtuals you can override that will be called when added via the DrawGridAPI. You usually do not need to manually construct this yourself, as DrawGridAPI::addDraw will construct one for you and return it.

```cpp
void setID(const std::string& id)
```
Sets the ID of the DrawNode.

```cpp
std::string getID() const
```
Returns the ID of the DrawNode.

```cpp
void setEnabled(bool enabled)
```
Sets if the DrawNode is enabled (should draw to the DrawGridLayer).

```cpp
bool isEnabled() const
```
Returns true if the DrawNode is enabled (will draw to the DrawGridLayer).

```cpp
void setZOrder(int order)
```
Sets the Z order of the DrawNode, note that this is width exclusive, it will draw in this order per line width, then rectangles, then rectangle outlines. 

```cpp
int getZOrder() const
```
Returns the Z order of the DrawNode.

```cpp
virtual void init(DrawGridLayer* drawGridLayer)
```
A virtual you can override that will be called whenever the DrawGridLayer is created.

```cpp
virtual void draw(DrawGridLayer* drawGridLayer, float minX, float maxX, float minY, float maxY);
```
A virtual you can override that will draw what is within it to the DrawGridLayer, The minX, maxX, minY, and maxY params are the culling bounds, these respect scale and rotation, please use these for the best performance.

## DrawLayers.hpp

Every method here has an optional priority param, it is used if you need to run your code after or before something else. (For static changes such `setColor` methods, the highest number is the only one that ends up getting set. `setColorFor` methods will be in order of prio and all will run).

### **`class Grid : public DrawNode`**

The editor grid.

```cpp
void setGridColor(const LineColor& color, int priority = 0)
```
Sets the grid color.

```cpp
const LineColor& getGridColor() const
```
Returns the grid color.

```cpp
int getGridColorPriority() const
```
Returns the grid color priority.

### **`class Bounds : public DrawNode`**

The bounds of the editor (white vertical line at X: 0 and the max and minimum height).

```cpp
void setTopBoundColor(const LineColor& color, int priority = 0)
```
Sets the top bound color.

```cpp
const LineColor& getTopBoundColor() const
```
Returns the top bound color.

```cpp
int getTopBoundColorPriority() const
```
Returns the top bound priority.

```cpp
void setBottomBoundColor(const LineColor& color, int priority = 0)
```
Sets the bottom bound color.

```cpp
const LineColor& getBottomBoundColor() const
```
Returns the bottom bound color.

```cpp
int getBottomBoundColorPriority() const
```
Returns the bottom bound priority.

```cpp
void setVerticalBoundColor(const LineColor& color, int priority = 0)
```
Sets the vertical bound color.

```cpp
const LineColor& getVerticalBoundColor() const
```
Returns the vertical bound color.

```cpp
int getVerticalBoundColorPriority() const
```
Returns the vertical bound priority.
 
### **`class Ground : public DrawNode`**

The ground lines (when in a non cube/robot gamemode).

```cpp
void setTopGroundColor(const LineColor& color, int priority = 0)
```
Sets the top ground color.

```cpp
const LineColor& getTopGroundColor() const
```
Returns the top ground color.

```cpp
int getTopGroundColorPriority() const
```
Returns the top ground priority.

```cpp
void setBottomGroundColor(const LineColor& color, int priority = 0)
```
Sets the bottom ground color.

```cpp
const LineColor& getBottomGroundColor() const
```
Returns the bottom ground color.

```cpp
int getBottomGroundColorPriority() const
```
Returns the bottom ground priority.

### **`class GuideObjects : public DrawNode`**

These are the objects that show portal bounds when preview is enabled.

```cpp
void setColorForObject(std::function<void(LineColor& bottomColor, LineColor& topColor, EffectGameObject* object)> colorForObject, int priority = 0)
```
Lets you pass in a method that allows for modifying the colors, as well as passing in the object. An example would be setting colors depending on an object.

### **`class EffectLines : public DrawNode`**

These are the lines triggers show when not spawn or touch triggered.

```cpp
void setColorForObject(std::function<void(LineColor& color, EffectGameObject* object)> colorForObject, int priority = 0)
```
Lets you pass in a method that allows for modifying the color, as well as passing in the object. An example would be setting colors depending on an object.

### **`class DurationLines : public DrawNode`**

These are the duration lines triggers show.

```cpp
void setColorForObject(std::function<void(LineColor& color, EffectGameObject* object)> colorForObject, int priority = 0)
```
Lets you pass in a method that allows for modifying the color, as well as passing in the object. An example would be setting colors depending on an object.

### **`class Guidelines : public DrawNode`**

These are the music guidelines you can set when choosing a song.

```cpp
void setColorForValue(std::function<void(LineColor& color, float value)> colorForValue, int priority = 0)
```
Lets you pass in a method that allows for modifying the color, as well as passing in the numeric color value the guideline is set to (view https://wyliemaster.github.io/gddocs/#/resources/client/level-components/guideline-string for more info). An example would be setting colors depending on the value, expanding what color values already exist.

### **`class BPMTriggers : public DrawNode`**

These are the bpm guidelines you can set with a BPM Trigger.

```cpp
void setColorForBeats(std::function<void(LineColor& color, AudioLineGuideGameObject* object, int beat, int beatsPerBar)> colorForBeats, int priority = 0)
```
Lets you pass in a method that allows for modifying the color, as well as passing in the object, what beat the line being modified is on, and how many beats per bar there are. An example would be changing the beat color depending on what beat it is in a bar.

### **`class AudioLine : public DrawNode`**

This is the line that shows when you play music in the editor.

```cpp
void setColorForTime(std::function<void(LineColor& color, bool playback, float time, const cocos2d::CCPoint& position)> colorForTime, int priority = 0);
```
Lets you pass in a method that allows for modifying the color, as well as passing in if playback is live, the song's time, and the position of the line. An example would be setting the line color hue based on the time. 

### **`class PositionLines : public DrawNode`**

The center vertical and horizontal lines that mark before and after a trigger activates.

```cpp
void setVerticalLineColor(const LineColor& color, int priority = 0)
```
Sets the vertical line color.

```cpp
const LineColor& getVerticalLineColor() const
```
Returns the vertical line color.

```cpp
int getVerticalLineColorPriority() const
```
Returns the vertical line priority.

```cpp
void setHorizontalLineColor(const LineColor& color, int priority = 0)
```
Sets the horizontal line color.

```cpp
const LineColor& getVerticalLineColor() const
```
Returns the horizontal line color.

```cpp
int getHorizontalLineColorPriority() const
```
Returns the horizontal line priority.

### **`class PreviewLockLine : public DrawNode`**

The line that shows the position where preview is locked.

```cpp
void setLineColor(const LineColor& color, int priority = 0)
```
Sets the line color.

```cpp
const LineColor& getLineColor() const
```
Returns the line color.

```cpp
int getLineColorPriority() const
```
Returns the line color priority.