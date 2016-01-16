;(function($) {
  $.fn.overlabel=function(options){var opts=$.extend({},$.fn.overlabel.defaults,options);var selection=this.filter('label[for]').map(function(){var label=$(this);var id=label.attr('for');var field=document.getElementById(id);if(!field)return;var o=$.meta?$.extend({},opts,label.data()):opts;label.addClass(o.label_class);var hide_label=function(){label.css(o.hide_css)};var show_label=function(){this.value||label.css(o.show_css)};$(field).parent().addClass(o.wrapper_class).end().focus(hide_label).blur(show_label).each(hide_label).each(show_label);return this});return opts.filter?selection:selection.end()};$.fn.overlabel.defaults={label_class:'overlabel-apply',wrapper_class:'overlabel-wrapper',hide_css:{'text-indent':'-10000px'},show_css:{'text-indent':'0px','cursor':'text'},filter:false};
})(jQuery);

/**
 * Invocation for overlabel:
 */

jQuery(document).ready(function() {
  $(".block-search label").overlabel();
});