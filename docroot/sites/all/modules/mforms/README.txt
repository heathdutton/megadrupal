================================================================================
ABOUT Mforms
================================================================================
Mforms module is a set of tools that help to create advanced multistep forms in
Drupal. It can be modified and extended so a developer has full control of the
way how individual form steps will be handeled, how and where will be between
form steps data stored, which step will follow, how form controls will act and
how the output will be displayed.

The main differences from existing multistep solutions for Drupal are
robustness, flexibility and extensibility. You can even dynamically change the
steps sequence and jump from one step to another regardless of their order.

================================================================================
FEATURES
================================================================================
- Statemachine that handles steps flow - no need for "iffing" logic of which
step is to follow.
- Store that keeps data between form steps. Currently available session store
and $form_state store.
- Three different out-of-the-box form controls.
- You can easily extend existing controls implementations to alter its behavior.
- Full compatibility with with Drupal form API.
- Each form step carries info of which step is current and which is previous
and next.
- Theming is simple as mforms provide css classes with current status. So
creating a cool looking web wizard with progress indicator is just a matter of
use of mforms and knowledge of css.
- Ajax support out of the box - that allows to build multistep forms i.e. in
modal window or just provide a cool functionality for end user.

================================================================================
INSTALLATION AND CONFIGURATION
================================================================================
Mforms is a library, so by itself it does not provide any functionality (besides
the real-life examples in the mforms example module). Therefore there is no
configuration or installation besides enabling the module in the module list.

================================================================================
BASIC USAGE
================================================================================
Included module mforms_example contains several real life examples that are
documented in code. To find out what it does visit /mforms page after you enable
mforms_example module.

Mforms architecture
--------------------------------------------------------------------------------
For faster dive into mforms here is the basic architecture:

Prerequisites
.............
STORE_KEY - not just identifies the storage slot for submitted values, it
represents the identifier of whole multi-step form implementation.

Steps file - The file where individual form steps with their validate and submit
callbacks reside. See mforms_example/mforms/
mforms_example.session_store_example.inc for demonstration. This file must be
named in the following pattern: MODULE_NAME.STORE_KEY.inc.

Store object - MformsIstore implementation responsible for storing submitted
values and internal mforms data needed to control the stepping process.

Steps object - it is aware of all steps, current step, next step and previous
step.

Controls object - the UI control object. It controls the form buttons.

Mforms initialization
.....................
Initialization process consists of instantiation of store, steps and controls
objects and calling mforms_init_module() function into which you pass
instantiated objects. You can do this in a Drupal page callback followed by
call of drupal_get_form(). However this approach will not allow you to handle
ajax calls.

The proper way is to implement hook_STORE_KEY_mforms_init() and
within this hook implementation do all initialization - see
mforms_example.module, function mforms_example_fs_store_example_mforms_init().

Drupal form callbacks
.....................
The form lifecycle is handeled by Drupal. Thls
erefore there must be regular form
callbacks: FORM_ID(), FORM_ID_validate() and FORM_ID_sbumit(). Inside these
mforms takes over as can be seen in the mforms_example.pages.inc, i.e. functions
mforms_example_fs_form[validate, submit]().

================================================================================
SPONSOR
================================================================================

This module is a product of blueMinds (http://blueminds.eu) web development
company.
