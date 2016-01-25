<?php
/**
 * Implements template_preprocess_page
 * @param  array $variables
 */
 function acme_preprocess_page(&$variables) {
  /* Makes the copyright date dynamic */
  $variables['copyright'] = t("Copyright " . date('Y'));

  /* Create a custom welcome box for users */
  /* If the visitor is not logged in... */
  if(isset($variables['user']->roles[1])){
    /* Set the message with a link to login */
    $variables['welcome'] = t("Welcome guest. " . l('Login?','user'));
  }
  /* Else (if the visitor is logged in)... */
  else{
    /* Pull the username from the variables array and link to profile */
    $username = l($variables['user']->name,'user');
    /* Set the message with their username and an option to log out */
    $variables['welcome'] = t("Welcome $username. " . l('Logout?','user/logout'));
  }
}
