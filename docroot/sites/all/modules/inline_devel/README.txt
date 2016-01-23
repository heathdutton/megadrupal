            Inline Devel - what is it?
Inline Devel is a module that extend the regular devel executed form with an
IDE options.

Why should you use it?
The execute code form is good for debugging data without edit files.
The problem is that we don't have an auto complete for functions and classes.
Inline devel offers you this solution. When starting to type the function name,
you'l get a list of functions, classes, interfaces and hooks available to you.

Yes, you can define hooks. All the hooks you'l write down in the execute code
form will be attached as hooks defined by the module inline_dvel_dummy_module.
If you worried - inline devel is not writing on the files of the modules but
create a session with a stack of the hooks you defnied or any other functions.

How to use it?
Becuase this module extend the devel execute form you need to go the form.
Go to ?q=devel/php. In the form start to type your code and you'l get the list
of available functions, classes, interfaces and hooks.

If you'd like to delete some functions that you defined in the execute code
form emable first the module "Inline devel UI" and then go to
?q=admin/config/development/inline_devel_admin and there you can delete the
functions.

Problem for beta:
1.  The regex pattern that detect functions from the form can detect only one
    function at a time. Also if there is a defined function and simple code
    after that - not work as well.

Future ideas:
* All code written in the execute code form - will be saved. Good for when
  closing the window without notice.

* Get list of the variables - drupal variables and variables that defined in
  other parts of drupal.
