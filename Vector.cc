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
       * @param[in] aNumOfElements is a std::size_t containing num of elements to init
       * @param[in] aInit is a const Type_t to initilize all slots in Vector to
       * @return fully constucted vector
       * @throws If exception occurs when filling vector memory block
      */
      Vector(std::size_t aNumOfElements, const Type_t* aInit) :
         mStart(new Type_t[aNumOfElements]),
         mNumOfElements(aNumOfElements),
         mCapacity(aNumOfElements)
      {
         try 
         {
            std::fill(begin(), end(), aInit);
         }
         catch (...)
         {
            delete[] mStart;
            throw;
         }
      }         

      /**
       * @brief Copy constructor
       * @param[in] aOther is Vector to copy from
       * @return fully constructed Vector
      */
      Vector(const Vector &aOther) :
         mStart(new Type_t[aOther.size()]),
         mNumOfElements(aOther.size()),
         mCapacity(aOther.capacity())
      {
         try 
         {
            std::copy(aOther.begin(), aOther.end(), begin());
         }
         catch (...)
         {
            delete[] mStart;
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
         delete[] mStart;
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

         mStart[size()] = aValue;
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

         mStart[size()] = std::move(aValue);
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

         mStart[size()] = std::move(Type_t(std::forward<Args>(aListOfArgs)...));
         ++mNumOfElements;
         return back();
      }

      /**
       * @brief Resizes the container to accomadate a certain number of elements
       * @param[in] aNumOfElements is a std::size_t
      */
      void resize(std::size_t aNumOfElements)
      {
         if (aNumOfElements <= capacity())
         {

         }
         else
         {
            Type_t* tNewBlock = new Type_t[aNumOfElements];

            if constexpr(std::is_nothrow_move_assignable_v<Type_t>)
            {
               std::move(begin(), end(), tNewBlock);
            }
            else try 
            {
               std::copy(begin(), end(), tNewBlock);
            }
            catch (...)
            {
               delete[] tNewBlock;
               throw;
            }

            delete[] mStart;
            mStart = tNewBlock;
            mCapacity = aNumOfElements;
         }
      }

      /**
       * @brief Insert a list of elements from any other container
       * @details We use iterators so this method can work with other containers. It 
       * takes a starting position to start the insertion from inserts values from aFirst to aLast from
       * other container into our Vector
       * 
      */
      template<class Iterator_t>
      Type_t* insert(const Type_t *aPosition, Iterator_t aFirst, Iterator_t aLast)
      {
         Type_t *tPosition = const_cast<Type_t*>(aPosition);

         const std::size_t tRemaining = capacity() - size();
         const std::size_t tDistance = std::distance(aFirst, aLast);

         if (tRemaining < tDistance)
         {
            // re-compute index
            std::size_t tIndex = std::distance(begin(), tPosition);
            resize(capacity() + tDistance - tRemaining);
            tPosition = std::next(begin(), tIndex);
         }

         // shifts existing elements to the right
         std::copy_backward(tPosition, end(), end() + tDistance);
         
         // regular copy
         std::copy(aFirst, aLast, tPosition);
         mNumOfElements += tDistance;
         return tPosition;
      }

      /**
       * @brief Erase an element from within the container
       * @param[in] aPosition is a const iterator
      */
      Type_t* erase(const Type_t* aPosition)
      {
         Type_t* tPosition = const_cast<Type_t*>(aPosition);

         if (tPosition == end())
         {
            return tPosition;
         }
         else
         {
            // Copies [aPosition + 1, end()] elements back into a position
            std::copy(std::next(aPosition), end(), aPosition);

            // there will be two dupes at the end of the vector, null out final element
            *std::prev(end()) = {};
            --mNumOfElements;
            return aPosition;
         }
      }

      private:

      /**
       * @brief Doubles the size of the vector
       * 
      */
      void grow() 
      {
         resize(capacity() * 2);
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