#ifndef __CALLBACK_HPP_
#define __CALLBACK_HPP_

#include <cstdint>



/**
 * GenericCallback is the base class for callbacks.
 * 
 */
template <typename ReturnType, typename... Args>
class GenericCallback
{
public:
    /** Finalizes an instance of the GenericCallback class. */
    virtual ~GenericCallback()
    {
    }

    /**
     * Calls the member function. Do not call execute unless isValid() returns true (ie. a
     * pointer to the object and the function has been set).
     *
     * @param  args This values set will be passed as the first argument in the function call.
     */
    virtual ReturnType execute(Args... args) = 0;

    /**
     * Function to check whether the Callback has been initialized with values.
     *
     * @return true If the callback is valid (i.e. safe to call execute).
     */
    virtual bool isValid() const = 0;
};




/**
 * A Callback is basically a wrapper of a pointer-to-member-function.
 *
 *
 * The class is templated in order to provide the class type of the object in which the
 * member function resides, and the argument types of the function to call.
 *
 *
 * @tparam dest_type The type of the class in which the member function resides.
 * @tparam Args      The list of types of the arguments in the member function.
 *
 * @note The member function to call must return void. The function can have zero, one, two or
 *       three arguments of any type.
 */

template <class dest_type, typename ReturnType, typename... Args>
struct Callback : public GenericCallback<ReturnType, Args...>
{
    /** Initializes a new instance of the Callback class. */
    Callback()
        : pobject(0), pmemfun(0)
    {
    }

    /**
     * Initializes a Callback with an object and a pointer to the member function in that object to
     * call.
     *
     * Initializes a Callback with an object and a pointer to the member function in that object to
     * call.
     *
     * @param [in] pObject   Pointer to the object on which the function should be called.
     * @param [in] pmemfun_ptr Address of member function. This is the version where function takes
     *                        three arguments.
     */
    Callback(dest_type* pObject, ReturnType (dest_type::*pmemfun_ptr)(Args... args))
        : pobject(pObject), pmemfun(pmemfun_ptr)
    {
    }

    /**
     * Calls the member function. Do not call execute unless isValid() returns true (ie. a
     * pointer to the object and the function has been set).
     *
     * @param  args This value will be passed as arguments in the function call.
     */
    virtual ReturnType execute(Args... args)
    {
        return (pobject->*pmemfun)(args ...);
    }

    /**
     * Function to check whether the Callback has been initialized with values.
     *
     * @return true If the callback is valid (i.e. safe to call execute).
     */
    virtual bool isValid() const
    {
        return (pobject != 0) && (pmemfun != 0);
    }

private:
    dest_type* pobject;
    ReturnType (dest_type::*pmemfun)(Args... args);
};


template<typename... Args>
using GenericCallbackNoReturn = GenericCallback<void, Args...>;


template<class dest_type, typename... Args>
using CallbackNoReturn = Callback<dest_type, void, Args...>;



#endif