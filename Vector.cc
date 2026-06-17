#include <utility>
#include <algorithm>



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
       * @brief Returns const pointer to beginning of Vector
       * @return Type_t * to start of memory
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
         mNumberOfElements(aNumOfElements),
         mCapacity(aNumOfElements)
      {
         try 
         {
            std::fill(begin(), end(), aInit)
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
            std::copy(aOther.begin(), aOther.end(), begin())
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
            std::copy(aListOfObjects.begin(), aListOfObjects.end(), begin())
         }
         catch (...)
         {
            delete[] mStart;
            throw;
         }
      }

      

      private:
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