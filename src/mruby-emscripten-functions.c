#include "mruby.h"
#include "mruby/compile.h"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>

void mrb_real_main_loop(void *arg)
{
  mrb_state *mrb = arg;
  //mrb_funcall(mrb, mrb_module_get(mrb, "Platform"), "main_loop", 0);
  mrb_load_string(mrb, "Platform.main_loop");
}

mrb_value mrb_set_main_loop(mrb_state *mrb, mrb_value self)
{
  emscripten_set_main_loop_arg(mrb_real_main_loop, &mrb, 0, 1);
  return mrb_nil_value();
}
#endif

void mrb_mruby_emscripten_functions_gem_init(mrb_state *mrb)
{
#ifdef __EMSCRIPTEN__
  mrb_define_module_function(mrb, mrb_module_get(mrb, "Platform"), "set_main_loop", mrb_set_main_loop, MRB_ARGS_NONE());
#endif
}

void mrb_mruby_emscripten_functions_gem_final(mrb_state *mrb)
{
  /* finalizer */
}
