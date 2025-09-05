#include "mruby.h"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
EM_JS(int, js_get_canvas_width, (), { return Module.canvas.width; });
EM_JS(int, js_get_canvas_height, (), { return Module.canvas.height; });

mrb_value mrb_get_canvas_width(mrb_state *mrb, mrb_value self)
{
  return mrb_int_value(mrb, js_get_canvas_width());
}

mrb_value mrb_get_canvas_height(mrb_state *mrb, mrb_value self)
{
  return mrb_int_value(mrb, js_get_canvas_height());
}

EM_JS(char *, js_get_attribute_from_element, (const char *selectorPointer, const char *attributePointer), {
  const selector = Module.UTF8ToString(selectorPointer);
  let attribute = Module.UTF8ToString(attributePointer);
  let value;

  if (attribute.slice(0, 5) === 'data-')
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

// This function requires FileSaver.js to be loaded on the webpage
// <script type='text/javascript' src="https://cdn.jsdelivr.net/gh/eligrey/FileSaver.js/dist/FileSaver.min.js"> </script>
EM_JS(void, saveFileFromMEMFSToDisk, (const char *memoryFSname, const char *localFSname),     // This can be called by C/C++ code
  {
    var isSafari = /^((?!chrome|android).)*safari/i.test(navigator.userAgent);
    var data = Module.FS.readFile(Module.UTF8ToString(memoryFSname));
    var blob;

    if (isSafari) blob = new Blob([data.buffer], { type: "application/octet-stream" });
    else blob = new Blob([data.buffer], { type: "application/octet-binary" });

    // NOTE: SaveAsDialog is a browser setting. For example, in Google Chrome,
    // in Settings/Advanced/Downloads section you have a setting:
    // 'Ask where to save each file before downloading' - which you can set true/false.
    // If you enable this setting it would always ask you and bring the SaveAsDialog
    saveAs(blob, Module.UTF8ToString(localFSname));
});

mrb_value mrb_save_from_memfs(mrb_state *mrb, mrb_value self)
{
  char *memfsname, *localfsname;
  mrb_get_args(mrb, "zz", &memfsname, &localfsname);

  saveFileFromMEMFSToDisk(memfsname, localfsname);

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

mrb_value mrb_exec_js(mrb_state *mrb, mrb_value self)
{
  char *key;
  mrb_get_args(mrb, "z", &key);
  emscripten_run_script(key);

  return mrb_nil_value();
}

void mrb_real_main_loop(void *mrb)
{
  struct RClass *platform = mrb_module_get(mrb, "Platform");
  struct RClass *emscripten = mrb_module_get_under(mrb, platform, "Emscripten");
  mrb_funcall(mrb, mrb_obj_value(emscripten), "main_loop", 0);
  // mrb_load_string(mrb, "Platform::Emscripten.main_loop");
}

mrb_value mrb_set_main_loop(mrb_state *mrb, mrb_value self)
{
  mrb_int fps = 0;
  mrb_get_args(mrb, "|i", &fps);
  emscripten_set_main_loop_arg(mrb_real_main_loop, mrb, fps, 1);
  return mrb_nil_value();
}
#endif
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windef.h>
#include <winbase.h>
#include <shtypes.h>
#include <wincon.h>

mrb_value mrb_mingw_attach_console(mrb_state *mrb, mrb_value self)
{
  // This allows us to write to a cmd.exe or powershell if we were run from
  // one, but otherwise don't open another window.
  if (AttachConsole(ATTACH_PARENT_PROCESS))
  {
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    return mrb_bool_value(1);
  }
  return mrb_bool_value(0);
}
#endif
void mrb_mruby_platform_functions_gem_init(mrb_state *mrb)
{
#ifdef __EMSCRIPTEN__
  struct RClass *Mruby_emscripten_platform_module;
  Mruby_emscripten_platform_module = mrb_define_module_under(mrb, mrb_module_get(mrb, "Platform"), "Emscripten");
  mrb_define_module_function(mrb, Mruby_emscripten_platform_module, "set_main_loop", mrb_set_main_loop, MRB_ARGS_OPT(1));
  mrb_define_module_function(mrb, Mruby_emscripten_platform_module, "exec_js", mrb_exec_js, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, Mruby_emscripten_platform_module, "get_canvas_width", mrb_get_canvas_width, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, Mruby_emscripten_platform_module, "get_canvas_height", mrb_get_canvas_height, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, Mruby_emscripten_platform_module, "set_local_storage", mrb_local_storage_set_item, MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, Mruby_emscripten_platform_module, "get_local_storage", mrb_local_storage_get_item, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, Mruby_emscripten_platform_module, "save_from_memfs", mrb_save_from_memfs, MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, Mruby_emscripten_platform_module, "get_attribute_from_element", mrb_get_attribute_from_element, MRB_ARGS_REQ(2));
#endif
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  struct RClass *Mruby_windows_platform_module;
  Mruby_windows_platform_module = mrb_define_module_under(mrb, mrb_module_get(mrb, "Platform"), "Windows");
  mrb_define_module_function(mrb, Mruby_windows_platform_module, "attach_console", mrb_mingw_attach_console, MRB_ARGS_NONE());
#endif
}

void mrb_mruby_platform_functions_gem_final(mrb_state *mrb)
{
  /* finalizer */
}
