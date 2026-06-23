#include <cstddef>
#include <algorithm>
#include <utility>
#include <initializer_list>
#include <iterator>



template <class Type_t>
/* @brief My own impl of std::vector */
class Vector 
{
   public:

      /**
       * @brief Returns the number of elements in the vector
       * @return std::size_t containing number of elements
      */
      std::size_t size() const 
      {
         return mNumOfElements;
      }

      /**
       * @brief Returns the capacity of the vector
       * @return std::size_t containing capacity
      */
      std::size_t capacity() const 
      {
         return mCapacity;
      }

      /**
       * @brief Returns whether Vector is empty or not
       * @return True if empty else False
      */
      bool empty() const 
      {
         return size() != 0;
      }
      
      /**
       * @brief Returns pointer to beginning of Vector
       * @return Type_t * to start of memory
      */
      Type_t *begin()
      {
         return mStart;
      }

      /**
       * @brief Returns const pointer to beginning of Vector
       * @return const Type_t*
      */
      const Type_t *cbegin() const 
      {
         return begin();
      }

      /**
       * @brief Returns pointer to end of Vector
       * @return Type_t* to end of memory
      */
      Type_t *end() 
      {
         return begin() + size();
      }

      /**
       * @brief Returns const pointer to end of Vector
       * @return const Type_t* to end of memory
       * 
      */
      const Type_t *end() const
      {
         return begin() + size();
      }

      /**
       * @brief Returns const pointer to end of Vector
       * @return const Type_t* to end of memory
       * 
      */
      const Type_t *cend() const
      {
         return end();
      }

      /**
       * @brief Default ctr
      */
      Vector() = default;

      /**
       * @brief Overloaded Ctr
       * @details To avoid default construction plus copy operation we malloc the necessary size and
       * then construct our aInit objects directly in the memory block
       * @param[in] aNumOfElements is a std::size_t containing num of elements to init
       * @param[in] aInit is a const Type_t to initilize all slots in Vector to
       * @return fully constucted vector
       * @throws If exception occurs when filling vector memory block
      */
      Vector(std::size_t aNumOfElements, const Type_t* aInit) :
         mStart(static_cast<Type_t*>(std::malloc(aNumOfElements * sizeof(Type_t)))),
         mNumOfElements(aNumOfElements),
         mCapacity(aNumOfElements)
      {
         try 
         {
            std::uninitialized_fill(begin(), end(), aInit);
         }
         catch (...)
         {
            std::free(mStart);
            throw;
         }
      }         

      /**
       * @brief Copy constructor
       * @param[in] aOther is Vector to copy from
       * @return fully constructed Vector
      */
      Vector(const Vector &aOther) :
         mStart(static_cast<Type_t*>(std::malloc(aOther.size() * sizeof(Type_t)))),
         mNumOfElements(aOther.size()),
         mCapacity(aOther.capacity())
      {
         try 
         {
            std::uninitialized_copy(aOther.begin(), aOther.end(), begin());
         }
         catch (...)
         {
            std::free(mStart);
            throw;
         }
      }

      /**
       * @brief Move constructor
       * @param[in] aOther is rhs Vector
       * @return fully constructed vector
      */
      Vector(Vector &&aOther) :
         mStart(std::exchange(aOther.mStart, nullptr)),
         mNumOfElements(std::exchange(aOther.mNumOfElements, 0)),
         mCapacity(std::exchange(aOther.mCapacity, 0))
      {}

      /**
       * @brief Initializer list constructor
       * @param[in] aListOfObjects
       * @return fully constructed Vector
      */
      Vector(std::initializer_list<Type_t> aListOfObjects) :
         mStart(new Type_t[aListOfObjects.size()]),
         mNumOfElements(aListOfObjects.size()),
         mCapacity(aListOfObjects.size())
      {
         try 
         {
            std::copy(aListOfObjects.begin(), aListOfObjects.end(), begin());
         }
         catch (...)
         {
            delete[] mStart;
            throw;
         }
      }

      /**
       * @brief Destructor
      */
      ~Vector()
      {
         std::__destroy(begin(), end());
         std::free(mStart);
      }

      /**
       * @brief Swaps two vectors 
       * @param[in] aOther is a lhs Vector
      */
      void swap(Vector &aOther) noexcept
      {
         std::swap(aOther.mStart, mStart);
         std::swap(aOther.mNumOfElements, mNumOfElements);
         std::swap(aOther.mCapacity, mCapacity);
      }

      /**
       * @brief Assignment operator (LHS Value)
       * @param[in] aOther is a Vector
      */
      Vector& operator=(const Vector &aOther)
      {
         Vector(aOther).swap(*this);
         return *this;
      }

      /**
       * @brief Assignment operator (RHS Value)
       * @param[in] aOther is a Vector
      */
      Vector& operator=(Vector &&aOther)
      {
         Vector(std::move(aOther)).swap(*this);
         return *this;
      }

      /**
       * @brief [] Operator
       * @param[in] aIndex is a std::size_t
       * @return Type_t& 
      */
      Type_t& operator[](std::size_t aIndex)
      {
         return mStart[aIndex];
      }

      /**
       * @brief [] Operator
       * @param[in] aIndex is a std::size_t
       * @return const Type_t& 
      */
      const Type_t& operator[](std::size_t aIndex) const
      {
         return mStart[aIndex];
      }

      /**
       * @brief Returns first element in vector
       * @return Type_t&
      */
      Type_t& front()
      {
         return (*this)[0];
      }

      /**
       * @brief Returns first element in vector
       * @return const Type_t&
      */
      const Type_t& front() const
      {
         return (*this)[0];
      }

      /**
       * @brief Returns the back element in vector
       * @return Type_t&
      */
      Type_t& back()
      {
         (*this)[size() - 1];
      }

      /**
       * @brief Returns the back element in vector
       * @return Type_t&
      */
      const Type_t& back() const 
      {
         (*this)[size() - 1];
      }

      /**
       * @brief Comparison operator for two Vectors
       * @param[in] aOther is a Vector
       * @return true if equal else false
      */
      bool operator==(const Vector& aOther) const
      {
         return (size() == aOther.size() && std::equal(begin(), end(), aOther.begin()));       
      }

      /**
       * @brief Comparison operator for two Vectors
       * @param[in] aOther is a Vector
       * @return true if equal else false
      */
      bool operator!=(const Vector& aOther) const
      {
         return !(*this == aOther);       
      }

      /**
       * @brief Adds element to the back of the vector
       * @param[in] aValue is a const Type_t
      */ 
      void push_back(const Type_t& aValue)
      {
         if (full())
         {
            grow();
         }

         std::__construct_at(end(), aValue);
         ++mNumOfElements;
      }

      /**
       * @brief Adds element to the back of the vector
       * @param[in] aValue is a RHS
      */ 
      void push_back(Type_t &&aValue)
      {
         if (full())
         {
            grow();
         }

         std::__construct_at(end(), std::move(aValue));
         ++mNumOfElements;
      }

      /**
       * @brief Constructs a Type_t element within the mem block of the vector
       * 
      */
      template<class ... Args>
      Type_t& emplace_back(Args &&...aListOfArgs)
      {
         if (full())
         {
            grow();
         }

         std::__construct_at(end(), std::move(Type_t(std::forward<Args>(aListOfArgs)...)));
         ++mNumOfElements;
         return back();
      }

      /**
       * @brief Resizes the container to accomadate a certain number of elements
       * @details Initializes excess space with default Type_t
       * @param[in] aNumOfElements is a std::size_t
      */
      void resize(std::size_t aNumOfElements)
      {
         if (aNumOfElements <= capacity())
         {

         }
         else
         {
            Type_t* tNewBlock = std::malloc(aNumOfElements * sizeof(Type_t));

            if constexpr(std::is_nothrow_move_assignable_v<Type_t>)
            {
               std::uninitialized_move(begin(), end(), tNewBlock);
            }
            else try 
            {
               std::uninitialized_copy(begin(), end(), tNewBlock);
            }
            catch (...)
            {
               std::free(tNewBlock);
               throw;
            }

            std::uninitialized_fill(tNewBlock + size(), tNewBlock + capacity(), Type_t{});
            std::destroy(begin(), end());
            std::free(mStart);
            mStart = tNewBlock;
            mNumOfElements = aNumOfElements;
            mCapacity = aNumOfElements;
         }
      }

      /**
       * @brief Insert a list of elements from another container.
       *
       * @details Uses iterators so this method can insert values from any compatible
       * container. Inserts the range [aFirst, aLast) starting at aPosition.
       */
      template<class Iterator_t>
      Type_t* insert(const Type_t* aPosition, Iterator_t aFirst, Iterator_t aLast)
      {
         Type_t* tPosition = const_cast<Type_t*>(aPosition);

         const std::size_t tDistance = std::distance(aFirst, aLast);

         // Nothing to insert.
         if (tDistance == 0)
         {
            return tPosition;
         }

         // Save the insertion index because reserve/reallocation may invalidate pointers.
         const std::size_t tIndex = std::distance(begin(), tPosition);

         // Number of existing elements after the insertion position.
         const std::size_t tElementsAfter = std::distance(tPosition, end());

         // Make sure we have enough capacity for the new elements.
         if (capacity() - size() < tDistance)
         {
            reserve(size() + tDistance);

            // Rebuild tPosition after reserve(), since reserve may reallocate.
            tPosition = std::next(begin(), tIndex);
         }

         Type_t* tOldEnd = end();
         Type_t* tNewEnd = tOldEnd + tDistance;

         /*
         * We need to create a gap of tDistance elements at tPosition.
         *
         * Before:
         * [ A B C D _ _ ]
         *       ^
         *       tPosition
         *
         * Insert X Y:
         *
         * After shifting:
         * [ A B _ _ C D ]
         *
         * Then copy X Y into the gap:
         * [ A B X Y C D ]
         */

         if (tElementsAfter > 0)
         {
            /*
            * First move/copy the last tDistance elements into uninitialized memory
            * at the end of the storage.
            *
            * This is needed because positions past the old end() do not contain
            * constructed Type_t objects yet.
            */
            const std::size_t tUninitializedCount =
               std::min(tDistance, tElementsAfter);

            Type_t* tUninitializedStart = tOldEnd - tUninitializedCount;

            if constexpr (std::is_nothrow_move_constructible_v<Type_t>)
            {
               std::uninitialized_move(tUninitializedStart, tOldEnd,
                                       tUninitializedStart + tDistance);
            }
            else
            {
               std::uninitialized_copy(tUninitializedStart, tOldEnd,
                                       tUninitializedStart + tDistance);
            }

            /*
            * Now shift the remaining already-constructed elements backward.
            *
            * copy_backward/move_backward is used because the source and destination
            * ranges overlap, and we are shifting to the right.
            */
            Type_t* tMoveEnd = tOldEnd - tUninitializedCount;

            std::move_backward(tPosition, tMoveEnd, tMoveEnd + tDistance);
         }

         /*
         * Copy the new elements into the gap.
         *
         * These positions already contain constructed objects, so regular copy
         * assignment is appropriate here.
         */
         std::copy(aFirst, aLast, tPosition);

         mNumOfElements += tDistance;

         return tPosition;
      }

      /**
       * @brief Erase one element from the container.
       * @param[in] aPosition Position of the element to erase.
       * @return Pointer to the element that shifted into the erased position,
       *         or end() if the erased element was the last element.
       */
      Type_t* erase(const Type_t* aPosition)
      {
         Type_t* tPosition = const_cast<Type_t*>(aPosition);

         // Erasing end() is a no-op.
         if (tPosition == end())
         {
            return tPosition;
         }

         // Shift every element after tPosition one slot to the left.
         std::copy(std::next(tPosition), end(), tPosition);

         // Clear the old duplicate final element.
         *std::prev(end()) = {};

         // Shrink logical size.
         --mNumOfElements;

         // Return iterator/pointer to the next valid element.
         return tPosition;
      }

      /**
       * @brief Adds more storage to container to accomadate more Type_t objects
       * @details Allocates a continguous block of memory of size aNewCap and moves/copies
       * over current objects from container into new space
       * @param[in] aNewCap is a std::size_t containing the new size of container
      */
      void reserve(std::size_t aNewCap)
      {
         if (aNewCap > capacity())
         {
            Type_t* tStart = static_cast<Type_t*>(std::malloc(aNewCap * sizeof(Type_t)));
            if constexpr(std::is_nothrow_move_assignable_v<Type_t>) 
            {
               std::uninitialized_move(begin(), end(), tStart);         
            } 
            else try 
            {
               std::uninitialized_copy(begin(), end(), tStart);
            } 
            catch (...) 
            {
               std::free(tStart);
               throw;
            }
         
            std::destroy(begin(), end());
            std::free(mStart);
            mStart = tStart;
            mCapacity = aNewCap;
         }
      }


      private:

      /**
       * @brief Doubles the size of the vector
       * 
      */
      void grow() 
      {
         reserve(capacity() ? capacity() * 2 : 16);
      }

      /* Pointer to the start of the vector */
      Type_t *mStart;

      /* Number of Elements in Vector */
      std::size_t mNumOfElements;

      /* Capacity of Vector */
      std::size_t mCapacity;

      /**
       * @brief Returns whether Vector is full or not
       * @return True if full else False
      */
      bool full() const 
      {
         return size() == capacity();
      }
};


int main()
{
   return 0;
}