Author: Noah Lively, KoreMedia Productions
Website: http://www.KoreMedia.net


PURPOSE:
This module was built as a helper for the frequent multi-step form wizards that 
we develop for our clients.  It makes wizard forms more flexible and more user-
friendly.


SUPPORT:
Paid support and integration services are available by contacting info@koremedia.net.

Not only can we help you integrate this module, but we can also develop custom
multi-step forms with this and other advanced features.



########################################

PREREQUISITES / WHAT TO EXPECT:

1. The CTools module must be installed on your site.  http://drupal.org/project/ctools
2. You will need to develop a wizard form and have a working knowledge of the CTools
   wizard form API.  Documentation can be found inside the CTools module docs.
3. You will need to do a small amount of coding to connect your wizard form to
   this module.

INSTALLATION / USAGE INSTRUCTIONS:

1. Enable this module. 
2. In your page callback function where you invoke your wizard form, add the 
   following line at the top of the function:

global $wizardsteps_wizard_info;

3. The ctools wizard form builder is invoked by calling a function with 3 arguments,
   something like this:
   ctools_wizard_multistep_form($form_info, $step, $form_state);

   Do not add the above line.  It should already be in your function if you have
   a working wizard form.

4. Just before this line, after your $form_info, $step and $form_state variables
   have been set up, add this code:

$wizardsteps_wizard_info = array(
  'form_info' => $form_info,
  'step' => $step,
  'form_state' => $form_state,
);


5. Create a mini panel (http://drupal.org/project/panels) that will serve as a
   block containing your wizard form steps.

   While setting up this panel add the Wizard Info context in the "context" screen.
  
   In the "content" screen, add the "Wizard Steps" content type.

6. Place your block in the desired theme region and configure the visibility rules
   so that it only shows up on your wizard form pages.