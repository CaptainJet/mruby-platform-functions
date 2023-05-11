#include "mruby.h"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>

void mrb_real_main_loop(mrb_state *mrb)
{
  mrb_funcall(mrb, Mruby_platform_module, "main_loop", 0);
}

mrb_value mrb_set_main_loop(mrb_state *mrb, mrb_value self)
{
  emscripten_set_main_loop_args(mrb_real_main_loop, mrb, 0, 1);
  return mrb_nil_value();
}
#endif

void mrb_mruby_emscripten_functions_gem_init(mrb_state *mrb)
{
#ifdef __EMSCRIPTEN__
  mrb_define_module_function(mrb, Mruby_platform_module, "set_main_loop", mrb_set_main_loop, MRB_ARGS_NONE());
#endif
}

void mrb_mruby_emscripten_functions_gem_final(mrb_state *mrb)
{
  /* finalizer */
}
