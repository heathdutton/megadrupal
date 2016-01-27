Are you a highly attractive person looking for good development tools? Welcome!
===============================================================================


How to contribute to "Tools for Highly Attractive People".
----------------------------------------------------------

Basically, all you need to do is add a file in the "includes" directory. By using `hap_include('myTool.inc')`, you will have access to any function in your contributed tool. That's it!

However.... If your tool requires implementing a hook, then you go through a few steps first:

1. Implement `hook_hap_info()` to describe your tool, similar to a module's .info file. You must define any modules or tools you depend on, if any. Also register which hooks your tool implements.

2. Implement the hook in your include using `hap_{myTool}` as your implementing module. So, for example, the Router tool will implement hook_menu like `hap_router_menu()`.

3. Add the hook in hap.hooks.inc if it doesn't already exist. Just follow suite with the existing format. Wrap the hook implementation with `hap_uses_hook()`, and use `hap_proxy()` in the body of the function.

4. Go to admin/config/hap/installer to enable your tool.

One note on registering callbacks within certain hooks (like `hook_menu`, or `hook_theme`), you must always provide the path and the file to be included. Use `hap_path()` to get the path of files in the includes, theme, or respective assets directory.


Modules vs. Tools -- How will I know?
-------------------------------------

*Disclaimer: these are not rock-solid rules, simply opinions that may be used as guidelines. This will forever be open to discussion.*

- If all you're adding are utility functions, it's probably a tool.
- If you're implementing hooks that get cached in some sort of registry (menu, theme, element_info, etc) than it's ok to be a tool.
- If implementing several "run-time" hooks (like when providing Fields) it's best to keep as modules.
- If implementing several "run-time" hooks but is only used rarely as a one-off situation (debugger tools) then either module or tool is fine.
- If something has to always be "on", then it should just be a module.
- If it is something really niche and not necessarily a general-purpose feature, then it shouldn't even be a part of the hap suite (Defer, Hub, Themepacket, etc)


A few cool conversation starters:
---------------------------------
- Hooks implemented in tools are lazy loaded.
- "Tools for Highly Attractive People" is responsible for at least 2 sub-systems: The proxy hook system, and the installer.
- Tools can require other modules (or other tools) before installing a tool.
- Modules can require tools, too! Just add `hap[] = some_tool_name` to your .info file.
