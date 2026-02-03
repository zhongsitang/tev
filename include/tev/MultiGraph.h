/*
 * tev -- the EDR viewer
 *
 * Copyright (C) 2025 Thomas Müller <contact@tom94.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// This file was adapted from the nanogui::Graph class, which was developed
// by Wenzel Jakob <wenzel.jakob@epfl.ch> and based on the NanoVG demo application
// by Mikko Mononen. Modifications were developed by Thomas Müller <contact@tom94.net>.

#pragma once

#include <tev/Common.h>

#include <nanogui/widget.h>

#include <functional>
#include <span>

namespace tev {

class MultiGraph : public nanogui::Widget {
public:
    MultiGraph(nanogui::Widget* parent, std::string_view caption = "Untitled");

    std::string_view caption() const { return mCaption; }
    void setCaption(std::string_view caption) { mCaption = caption; }

    std::string_view header() const { return mHeader; }
    void setHeader(std::string_view header) { mHeader = header; }

    std::string_view footer() const { return mFooter; }
    void setFooter(std::string_view footer) { mFooter = footer; }

    const nanogui::Color& backgroundColor() const { return mBackgroundColor; }
    void setBackgroundColor(const nanogui::Color& backgroundColor) { mBackgroundColor = backgroundColor; }

    const nanogui::Color& foregroundColor() const { return mForegroundColor; }
    void setForegroundColor(const nanogui::Color& foregroundColor) { mForegroundColor = foregroundColor; }

    const nanogui::Color& textColor() const { return mTextColor; }
    void setTextColor(const nanogui::Color& textColor) { mTextColor = textColor; }

    std::span<const float> values() const { return mValues; }
    void setValues(std::span<const float> values) { mValues = {values.begin(), values.end()}; }

    std::span<nanogui::Color> colors() { return mColors; }
    void setColors(std::span<const nanogui::Color> colors) { mColors = {colors.begin(), colors.end()}; }

    void setNChannels(int nChannels) { mNChannels = nChannels; }

    virtual nanogui::Vector2i preferred_size_impl(NVGcontext* ctx) const override;
    virtual void draw(NVGcontext* ctx) override;

    void setMinimum(float minimum) { mMinimum = minimum; }

    void setMean(float mean) { mMean = mean; }

    void setMaximum(float maximum) { mMaximum = maximum; }

    void setZero(int zeroBin) { mZeroBin = zeroBin; }

    // Range selector for histogram
    void setRangeSelectionEnabled(bool enabled) { mRangeSelectionEnabled = enabled; }
    bool rangeSelectionEnabled() const { return mRangeSelectionEnabled; }

    // Set the range in normalized coordinates [0, 1] relative to histogram bins
    void setRangeNormalized(float rangeMin, float rangeMax) {
        mRangeMin = rangeMin;
        mRangeMax = rangeMax;
    }

    float rangeMinNormalized() const { return mRangeMin; }
    float rangeMaxNormalized() const { return mRangeMax; }

    // Callback when range changes. Parameters are (rangeMin, rangeMax) in normalized coords.
    void setRangeCallback(const std::function<void(float, float)>& callback) { mRangeCallback = callback; }

    // Set min/max values for converting normalized range to actual values
    void setValueRange(float minValue, float maxValue) {
        mMinValue = minValue;
        mMaxValue = maxValue;
    }

    float minValue() const { return mMinValue; }
    float maxValue() const { return mMaxValue; }

    // Mouse event handlers
    bool mouse_button_event(const nanogui::Vector2i& p, int button, bool down, int modifiers) override;
    bool mouse_drag_event(const nanogui::Vector2i& p, const nanogui::Vector2i& rel, int button, int modifiers) override;
    bool mouse_motion_event(const nanogui::Vector2i& p, const nanogui::Vector2i& rel, int button, int modifiers) override;

protected:
    std::string mCaption, mHeader, mFooter;
    nanogui::Color mBackgroundColor, mForegroundColor, mTextColor;
    std::vector<float> mValues;
    std::vector<nanogui::Color> mColors;
    int mNChannels = 1;
    float mMinimum = 0, mMean = 0, mMaximum = 0;
    int mZeroBin = 0;

    // Range selector state
    bool mRangeSelectionEnabled = false;
    float mRangeMin = 0.0f;  // Normalized position [0, 1]
    float mRangeMax = 1.0f;  // Normalized position [0, 1]
    float mMinValue = 0.0f;  // Actual value at histogram min
    float mMaxValue = 1.0f;  // Actual value at histogram max

    std::function<void(float, float)> mRangeCallback;

    enum class DragMode { None, Left, Right, Middle };
    DragMode mDragMode = DragMode::None;
    float mDragStartRangeMin = 0.0f;
    float mDragStartRangeMax = 1.0f;
    int mDragStartX = 0;
};

} // namespace tev
