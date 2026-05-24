# Good Grid

## <cb>The ultimate DrawGridLayer rewrite and API!</c>

- <cg>Rotation Support!</c>
- <cg>Extensive API</c>
- <cg>Bug Fixes</c>
- <cg>Exactly Like Vanilla</c>

<cr>Editor Extension in MegaHack cannot be detected, thus the grid size will too small unless you enable extension override in this mod's settings.</c>

Here's what you can do with the API as a Developer:

Everything here is under the `good_grid` namespace.

## GoodGrid.hpp

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
bool isObjectVisible(GameObject* object)
```
Util to check if an object is visible (accounting for selecting, group and hide options).

## GradientColor.hpp

This class can store either 1 or 2 colors. It can be constructed just like a ccColor4B, and they are compatible with each other. When there is a second color, what is drawn will be done as a gradient of the two rather than one color.

## DrawGridBase.hpp

This class is what makes up every part of the new DrawGridLayer, it is a CCNode and thus can have its visibility and z order set. Every child on the DrawGridLayer inherits this class. It provides a few methods as well as some virtuals you can override that will be called when added to the DrawGridLayer as a child. This also contains a few of the API methods seen above for convenience.

```cpp
void drawLine(const cocos2d::ccVertex2F& start, const cocos2d::ccVertex2F& end, const GradientColor& color, float width, BlendMode mode = BlendMode::ADDITIVE)
```
Draws a line to the screen with two coordinates where it starts and ends. The width is the line width, for the purpose of optimization. BlendMode is an optional param that will let you change the blending between Additive, Multiply, and Invert.

```cpp
void drawRect(const cocos2d::CCRect& rect, const GradientColor& color, BlendMode mode = BlendMode::ADDITIVE)
```
Draws a rectangle to the screen with a CCRect param dictating the bounds and a color param to set its color. BlendMode is an optional param that will let you change the blending between Additive, Multiply, and Invert.

```cpp
void drawRectOutline(const cocos2d::CCRect& rect, const GradientColor& color, float width, BlendMode mode = BlendMode::ADDITIVE)
```
Draws a rectangle outline to the screen with a CCRect param dictating the bounds, a color param to set its color, and the width of the outline. BlendMode is an optional param that will let you change the blending between Additive, Multiply, and Invert.

```cpp
virtual void draw(float minX, float maxX, float minY, float maxY);
```
A virtual you can override that will draw what is within it to the DrawGridLayer, The minX, maxX, minY, and maxY params are the culling bounds, these respect scale and rotation, please use these for the best performance.

## DrawLayers

You can include all the draw layers by including `DrawLayers.hpp`

`setPropertiesFor` methods will be in order of priority set and all will run.

### **`class Grid : public DrawGridBase`**

The editor grid.

```cpp
void setGridColor(const GradientColor& color)
```
Sets the grid color.

```cpp
const GradientColor& getGridColor() const
```
Returns the grid color.

```cpp
void setLineWidth(float width)
```
Sets the line width.

```cpp
float getLineWidth() const
```
Returns the line width.

```cpp
void setInverted(bool inverted)
```
Sets the grid to be inverted or not.

```cpp
bool isInverted() const
```
Returns whether the grid is inverted or not.

### **`class Bounds : public DrawGridBase`**

The bounds of the editor (white vertical line at X: 0 and the max and minimum height).

```cpp
void setTopBoundColor(const GradientColor& color)
```
Sets the top bound color.

```cpp
const GradientColor& getTopBoundColor() const
```
Returns the top bound color.

```cpp
void setBottomBoundColor(const GradientColor& color)
```
Sets the bottom bound color.

```cpp
const GradientColor& getBottomBoundColor() const
```
Returns the bottom bound color.

```cpp
void setVerticalBoundColor(const GradientColor& color)
```
Sets the vertical bound color.

```cpp
const GradientColor& getVerticalBoundColor() const
```
Returns the vertical bound color.

```cpp
void seTopBoundtLineWidth(float width)
```
Sets the top bound line width.

```cpp
float getTopBoundLineWidth() const
```
Returns the top bounc line width.

```cpp
void setBottomBoundLineWidth(float width)
```
Sets the bottom bound line width.

```cpp
float getBottomBoundLineWidth() const
```
Returns the bottom bound line width.

```cpp
void setVerticalBoundLineWidth(float width)
```
Sets the vertical bound line width.

```cpp
float getVerticalBoundLineWidth() const
```
Returns the vertical bound line width.

### **`class Ground : public DrawGridBase`**

The ground lines (when in a non cube/robot gamemode).

```cpp
void setTopGroundColor(const GradientColor& color)
```
Sets the top ground color.

```cpp
const GradientColor& getTopGroundColor() const
```
Returns the top ground color.

```cpp
void setBottomGroundColor(const GradientColor& color)
```
Sets the bottom ground color.

```cpp
const GradientColor& getBottomGroundColor() const
```
Returns the bottom ground color.

```cpp
void setTopGroundLineWidth(float width)
```
Sets the top ground line width.

```cpp
float getTopGroundLineWidth() const
```
Returns the top ground line width.

```cpp
void setBottomGroundLineWidth(float width)
```
Sets the bottom ground line width.

```cpp
float getBottomGroundLineWidth() const
```
Returns the bottom ground line width.

### **`class GuideObjects : public DrawGridBase`**

These are the objects that show portal bounds when preview is enabled.

```cpp
void setPropertiesForObject(std::function<void(GradientColor& bottomColor, GradientColor& topColor, EffectGameObject* object, float& lineWidthBottom, float& lineWidthTop)> colorForObject, int priority = 0)
```
Lets you pass in a method that allows for modifying the colors, as well as passing in the object. An example would be setting colors depending on an object.

### **`class EffectLines : public DrawGridBase`**

These are the lines triggers show when not spawn or touch triggered.

```cpp
void setPropertiesForObject(std::function<void(GradientColor& color, EffectGameObject* object, float& lineWidth)> colorForObject, int priority = 0)
```
Lets you pass in a method that allows for modifying the color, as well as passing in the object. An example would be setting colors depending on an object.

### **`class DurationLines : public DrawGridBase`**

These are the duration lines triggers show.

```cpp
void setPropertiesForObject(std::function<void(GradientColor& color, EffectGameObject* object, float& lineWidth)> colorForObject, int priority = 0)
```
Lets you pass in a method that allows for modifying the color, as well as passing in the object. An example would be setting colors depending on an object.

### **`class Guidelines : public DrawGridBase`**

These are the music guidelines you can set when choosing a song.

```cpp
void setPropertiesForValue(std::function<void(GradientColor& color, float value, float& lineWidth)> colorForValue, int priority = 0)
```
Lets you pass in a method that allows for modifying the color, as well as passing in the numeric color value the guideline is set to (view https://wyliemaster.github.io/gddocs/#/resources/client/level-components/guideline-string for more info). An example would be setting colors depending on the value, expanding what color values already exist.

### **`class BPMTriggers : public DrawGridBase`**

These are the bpm guidelines you can set with a BPM Trigger.

```cpp
void setPropertiesForBeats(std::function<void(GradientColor& color, AudioLineGuideGameObject* object, int beat, int beatsPerBar, float& lineWidth)> colorForBeats, int priority = 0)
```
Lets you pass in a method that allows for modifying the color, as well as passing in the object, what beat the line being modified is on, and how many beats per bar there are. An example would be changing the beat color depending on what beat it is in a bar.

### **`class AudioLine : public DrawGridBase`**

This is the line that shows when you play music in the editor.

```cpp
void setPropertiesForTime(std::function<void(GradientColor& color, bool playback, float time, const cocos2d::CCPoint& position, float& lineWidth)> colorForTime, int priority = 0);
```
Lets you pass in a method that allows for modifying the color, as well as passing in if playback is live, the song's time, and the position of the line. An example would be setting the line color hue based on the time. 

### **`class PositionLines : public DrawGridBase`**

The center vertical and horizontal lines that mark before and after a trigger activates.

```cpp
void setVerticalLineColor(const GradientColor& color)
```
Sets the vertical line color.

```cpp
const GradientColor& getVerticalLineColor() const
```
Returns the vertical line color.

```cpp
void setHorizontalLineColor(const GradientColor& color)
```
Sets the horizontal line color.

```cpp
const GradientColor& getVerticalLineColor() const
```
Returns the horizontal line color.

```cpp
void setVerticalLineWidth(float width)
```
Sets the vertical line width.

```cpp
float getVerticalLineWidth() const
```
Returns the vertical line width.

```cpp
void setHorizontalLineWidth(float width)
```
Sets the horizontal line width.

```cpp
float getHoritzontalLineWidth() const
```
Returns the horizontal line width.

### **`class PreviewLockLine : public DrawGridBase`**

The line that shows the position where preview is locked.

```cpp
void setLineColor(const GradientColor& color)
```
Sets the line color.

```cpp
const GradientColor& getLineColor() const
```
Returns the line color.

```cpp
void setLineWidth(float width)
```
Sets the line width.

```cpp
float getLineWidth() const
```
Returns the line width.