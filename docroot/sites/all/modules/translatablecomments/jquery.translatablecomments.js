// $Id: jquery.translatablecomments.js,v 1.7 2010/08/23 12:38:33 davetrainer Exp $

if (Drupal.jsEnabled) {
  $(document).ready(function (){

    $(selectors).each(function (i) {
      var source = $(this);
      var wrapper = Drupal.theme('translatorWrapper').insertBefore(this);      
      $.each(languages, function (code, language){
        wrapper.append(Drupal.theme('translator', code, language).click(function () {
          $.translate(source.html(), code, function(translation){
            source.html(translation);
          });
          this.blur();
          return false;
        }));
      });
    });
  });
}

/**
 * Theme functions.
 * 
 * You can override these functions in your theme. See http://drupal.org/node/171213
 */
Drupal.theme.prototype.translatorWrapper = function () {
  return $("<span></span>").attr("class","translator");
}

Drupal.theme.prototype.translator = function (code, language) {
  return $("<a />").html(code).attr("href", "#").attr("class","translator");
}
