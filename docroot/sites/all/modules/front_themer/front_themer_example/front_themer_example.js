/**
 * A toy example of how theme implementations generated by front_themer can be called in JS
 */
(function ($) {
  Drupal.behaviors.frontThemer = {
    attach: function(context, settings) {
      var elem = Drupal.theme('example_visible', 'visibleClass', 'Visible title!', 'Visible content!');
      var elem2 = Drupal.theme('example_hidden', 'invisibleClass', 'inVisible title!', 'inVisible content!');
      var elem3 = Drupal.theme('example_defaults');

      console.log("HTML for theme example_visible", elem);
      console.log("HTML for theme example_hidden", elem2);
      console.log("HTML for theme example_defaults", elem3);
    }
  }
}(jQuery));