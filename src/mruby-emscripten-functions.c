#include "mruby.h"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
EM_JS(char *, js_get_attribute_from_element, (const char *selectorPointer, const char *attributePointer), {
  const selector = Module.UTF8ToString(selectorPointer);
  let attribute = Module.UTF8ToString(attributePointer);
  let value;

  if (attribute.slice(0, 5) == = 'data-')
  {
    attribute = attribute.slice(5, attribute.length);
    value = document.querySelector(selector).dataset[attribute];
  }
  else
  {
    value = document.querySelector(selector)[attribute];
  }

  const byteCount = Module.lengthBytesUTF8(value) + 1;
  const valuePointer = Module._malloc(byteCount);

  Module.stringToUTF8(value, valuePointer, byteCount);

  return valuePointer;
});

mrb_value mrb_get_attribute_from_element(mrb_state *mrb, mrb_value self)
{
  char *selector, *attribute;
  mrb_get_args(mrb, "zz", &selector, &attribute);

  return mrb_str_new_cstr(mrb, js_get_attribute_from_element(selector, attribute));
}

EM_JS(void, js_local_storage_set_item, (const char *keyPointer, const char *valuePointer), {
  const key = Module.UTF8ToString(keyPointer);
  const value = Module.UTF8ToString(valuePointer);

  console.log(key, value);
  localStorage.setItem(key, value);
});

mrb_value mrb_local_storage_set_item(mrb_state *mrb, mrb_value self)
{
  char *key, *value;
  mrb_get_args(mrb, "zz", &key, &value);

  js_local_storage_set_item(key, value);

  return mrb_nil_value();
}

EM_JS(char *, js_local_storage_get_item, (const char *keyPointer), {
  const key = Module.UTF8ToString(keyPointer);

  // We can't return two different types, so let's just return an empty string,
  // this shouldn't be too bad an edge case.
  const value = localStorage.getItem(key) || "";

  const byteCount = Module.lengthBytesUTF8(value) + 1;
  const valuePointer = Module._malloc(byteCount);

  Module.stringToUTF8(value, valuePointer, byteCount);

  return valuePointer;
});

mrb_value mrb_local_storage_get_item(mrb_state *mrb, mrb_value self)
{
  char *key;
  mrb_get_args(mrb, "z", &key);

  return mrb_str_new_cstr(mrb, js_local_storage_get_item(key));
}

void mrb_real_main_loop(void *arg)
{
  mrb_state *mrb = arg;
  struct RClass *platform = mrb_module_get_under(mrb, mrb_module_get(mrb, "Platform"), "Emscripten");
  mrb_funcall(mrb, mrb_obj_value(platform), "main_loop", 0);
  // mrb_load_string(mrb, "Platform::Emscripten.main_loop");
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
  struct RClass *Mruby_emscripten_platform_module;
  Mruby_emscripten_platform_module = mrb_define_module_under(mrb, mrb_module_get(mrb, "Platform"), "Emscripten");
  mrb_define_module_function(mrb, Mruby_emscripten_platform_module, "set_main_loop", mrb_set_main_loop, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, Mruby_emscripten_platform_module, "set_local_storage", mrb_local_storage_set_item, MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, Mruby_emscripten_platform_module, "get_local_storage", mrb_local_storage_get_item, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, Mruby_emscripten_platform_module, "get_attribute_from_element", mrb_get_attribute_from_element, MRB_ARGS_REQ(2));
#endif
}

void mrb_mruby_emscripten_functions_gem_final(mrb_state *mrb)
{
  /* finalizer */
}
