/*!
 * @file  callback.hpp
 * @brief Callback handler
 */
#ifndef CLI_BASIC_ENGINE_CALLBACK_HPP
#define CLI_BASIC_ENGINE_CALLBACK_HPP

#include <cassert>


namespace cli {

  class Command;

  //! Abstract callback function wrapper
  class CallbackFunction {
    public:
    /*!
     * @brief  virtual dtor.
     * @note   Custom dtor. must be implemented in every derived class.
     */
    virtual ~CallbackFunction() noexcept = default;
    /*!
     * @brief      Call operator
     * @param[in]  command : command object
     * @note       Response of the callback function is stored in the given command object.
     */
    virtual void operator()(Command& command) = 0;
  };


  /*!
   * @brief  Callback function wrapper for static functions
   *
   * @code
   * // Usage
   * void func( Command& ) noexcept;
   *
   * CallbackStaticFunction callback( &func );
   *
   * Command command;
   * callback( command );
   * @endcode
   */
  class CallbackStaticFunction final : public CallbackFunction {

    public:
    /*!
     * @typedef  function_t
     * @brief    type of callback function
     */
    using function_t = void(*)(Command&);

    /*!
     * @brief      ctor.
     * @param[in]  cbf : pointer to callback function
     */
    explicit CallbackStaticFunction(typename CallbackStaticFunction::function_t cbf) noexcept
      : _function(cbf)
    {
      assert(is_available());
    }

    //! dtor.
    ~CallbackStaticFunction() noexcept override
    {
      _function = nullptr;
    }

    //! Call operator
    void operator()(Command& command) override
    {
      assert(is_available()), _function(command);
    }

    protected:
    //! Check validation
    bool is_available() noexcept
    {
      return _function != nullptr;
    }

    private:
    function_t _function;

  };


  /*!
   * @brief   Callback function wrapper for member functions in the Engine class
   * @tparam  Engine : class name which has the callback function as a member
   *
   * @code
   * // Usage
   * class MyEngine {
   *   void on_call( Command& ) noexcept;
   * };
   *
   * MyEngine engine;
   * CallbackMemberFunction callback( &engine, &Engine::*on_call );
   *
   * Command command;
   * callback( command );  // Don't have to give the pointer to the engine instance
   * @endcode
   */
  template<class Engine>
  class CallbackMemberFunction final : public CallbackFunction {

    public:
    /*!
     * @typedef  function_t
     * @brief    type of callback function
     */
    using function_t = void(Engine::*)(Command&);

    /*!
     * @brief      ctor.
     * @param[in]  engine : pointer to Engine instance
     * @param[in]  cbf    : pointer to callback member function
     */
    CallbackMemberFunction(Engine* const engine,
                           typename CallbackMemberFunction<Engine>::function_t cbf) noexcept
      : _engine(engine), _function(cbf)
    {
      assert(is_available());
    }

    //! dtor.
    ~CallbackMemberFunction() noexcept override
    {
      _engine = nullptr, _function = nullptr;
    }

    //! Call operator
    void operator()(Command& command) override
    {
      assert(is_available()), (_engine->*_function)(command);
    }

    protected:
    //! Check validation
    bool is_available() noexcept
    {
      return _engine != nullptr && _function != nullptr;
    }

    private:
    Engine*     _engine;
    function_t  _function;

  };

}

#endif  /* CLI_BASIC_ENGINE_CALLBACK_HPP */
