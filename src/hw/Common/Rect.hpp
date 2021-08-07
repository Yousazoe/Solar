#pragma once
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Vector2.hpp"
#include <algorithm>


namespace hw
{
////////////////////////////////////////////////////////////
/// \brief Utility class for manipulating 2D axis aligned rectangles
///
////////////////////////////////////////////////////////////
template <typename T>
class Rect
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty rectangle (it is equivalent to calling
    /// Rect(0, 0, 0, 0)).
    ///
    ////////////////////////////////////////////////////////////
    Rect();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the rectangle from its coordinates
    ///
    /// Be careful, the last two parameters are the width
    /// and height, not the right and bottom coordinates!
    ///
    /// \param rectLeft   Left coordinate of the rectangle
    /// \param rectTop    Top coordinate of the rectangle
    /// \param rectWidth  Width of the rectangle
    /// \param rectHeight Height of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    Rect(T rectLeft, T rectTop, T rectWidth, T rectHeight);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the rectangle from position and size
    ///
    /// Be careful, the last parameter is the size,
    /// not the bottom-right corner!
    ///
    /// \param position Position of the top-left corner of the rectangle
    /// \param size     Size of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    Rect(const Vector2<T>& position, const Vector2<T>& size);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the rectangle from another type of rectangle
    ///
    /// This constructor doesn't replace the copy constructor,
    /// it's called only when U != T.
    /// A call to this constructor will fail to compile if U
    /// is not convertible to T.
    ///
    /// \param rectangle Rectangle to convert
    ///
    ////////////////////////////////////////////////////////////
    template <typename U>
    explicit Rect(const Rect<U>& rectangle);

    ////////////////////////////////////////////////////////////
    /// \brief Check if a point is inside the rectangle's area
    ///
    /// This check is non-inclusive. If the point lies on the
    /// edge of the rectangle, this function will return false.
    ///
    /// \param x X coordinate of the point to test
    /// \param y Y coordinate of the point to test
    ///
    /// \return True if the point is inside, false otherwise
    ///
    /// \see intersects
    ///
    ////////////////////////////////////////////////////////////
    bool contains(T x, T y) const;

    ////////////////////////////////////////////////////////////
    /// \brief Check if a point is inside the rectangle's area
    ///
    /// This check is non-inclusive. If the point lies on the
    /// edge of the rectangle, this function will return false.
    ///
    /// \param point Point to test
    ///
    /// \return True if the point is inside, false otherwise
    ///
    /// \see intersects
    ///
    ////////////////////////////////////////////////////////////
    bool contains(const Vector2<T>& point) const;

    ////////////////////////////////////////////////////////////
    /// \brief Check the intersection between two rectangles
    ///
    /// \param rectangle Rectangle to test
    ///
    /// \return True if rectangles overlap, false otherwise
    ///
    /// \see contains
    ///
    ////////////////////////////////////////////////////////////
    bool intersects(const Rect<T>& rectangle) const;

    ////////////////////////////////////////////////////////////
    /// \brief Check the intersection between two rectangles
    ///
    /// This overload returns the overlapped rectangle in the
    /// \a intersection parameter.
    ///
    /// \param rectangle    Rectangle to test
    /// \param intersection Rectangle to be filled with the intersection
    ///
    /// \return True if rectangles overlap, false otherwise
    ///
    /// \see contains
    ///
    ////////////////////////////////////////////////////////////
    bool intersects(const Rect<T>& rectangle, Rect<T>& intersection) const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    T left;   ///< Left coordinate of the rectangle
    T top;    ///< Top coordinate of the rectangle
    T width;  ///< Width of the rectangle
    T height; ///< Height of the rectangle
};

////////////////////////////////////////////////////////////
/// \relates Rect
/// \brief Overload of binary operator ==
///
/// This operator compares strict equality between two rectangles.
///
/// \param left  Left operand (a rectangle)
/// \param right Right operand (a rectangle)
///
/// \return True if \a left is equal to \a right
///
////////////////////////////////////////////////////////////
template <typename T>
bool operator ==(const Rect<T>& left, const Rect<T>& right);

////////////////////////////////////////////////////////////
/// \relates Rect
/// \brief Overload of binary operator !=
///
/// This operator compares strict difference between two rectangles.
///
/// \param left  Left operand (a rectangle)
/// \param right Right operand (a rectangle)
///
/// \return True if \a left is not equal to \a right
///
////////////////////////////////////////////////////////////
template <typename T>
bool operator !=(const Rect<T>& left, const Rect<T>& right);

#include "Rect.inl"

// Create typedefs for the most common types
typedef Rect<int>   IntRect;
typedef Rect<float> FloatRect;

} // namespace hw




////////////////////////////////////////////////////////////
/// \class hw::Rect
/// \ingroup graphics
///
/// A rectangle is defined by its top-left corner and its size.
/// It is a very simple class defined for convenience, so
/// its member variables (left, top, width and height) are public
/// and can be accessed directly, just like the vector classes
/// (Vector2 and Vector3).
///
/// To keep things simple, hw::Rect doesn't define
/// functions to emulate the properties that are not directly
/// members (such as right, bottom, center, etc.), it rather
/// only provides intersection functions.
///
/// hw::Rect uses the usual rules for its boundaries:
/// \li The left and top edges are included in the rectangle's area
/// \li The right (left + width) and bottom (top + height) edges are excluded from the rectangle's area
///
/// This means that hw::IntRect(0, 0, 1, 1) and hw::IntRect(1, 1, 1, 1)
/// don't intersect.
///
/// hw::Rect is a template and may be used with any numeric type, but
/// for simplicity the instantiations used by SFML are typedef'd:
/// \li hw::Rect<int> is hw::IntRect
/// \li hw::Rect<float> is hw::FloatRect
///
/// So that you don't have to care about the template syntax.
///
/// Usage example:
/// \code
/// // Define a rectangle, located at (0, 0) with a size of 20x5
/// hw::IntRect r1(0, 0, 20, 5);
///
/// // Define another rectangle, located at (4, 2) with a size of 18x10
/// hw::Vector2i position(4, 2);
/// hw::Vector2i size(18, 10);
/// hw::IntRect r2(position, size);
///
/// // Test intersections with the point (3, 1)
/// bool b1 = r1.contains(3, 1); // true
/// bool b2 = r2.contains(3, 1); // false
///
/// // Test the intersection between r1 and r2
/// hw::IntRect result;
/// bool b3 = r1.intersects(r2, result); // true
/// // result == (4, 2, 16, 3)
/// \endcode
///
////////////////////////////////////////////////////////////
