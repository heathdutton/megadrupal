/**
 * @file
 * stacktable.module.js
 */

(function($) {
    Drupal.behaviors.stacktable = {
        attach: function(context, settings) {
            var table = settings.key.table;
            $(table).once("stacktable", function() {
                $(table, context).stacktable();
            });
        }
    };
}(jQuery));
