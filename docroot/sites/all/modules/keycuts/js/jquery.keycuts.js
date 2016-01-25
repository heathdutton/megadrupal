// keep all your code in a closure.
;(function($, window, document, undefined) {

    Drupal.behaviors.keyShortCuts = {
        attach: function (context, settings) {
            if(context == document) {
                for(index in settings.keycuts.shortcuts) {
                    var shortcut = settings.keycuts.shortcuts[index];
                    var sequences = shortcut.sequence.split(/[\s,]+/);
                    var action = shortcut.action;
                    var prevent_default = shortcut.prevent_default;

                    switch(action){
                    case 'trigger':
                        var selector = shortcut.arguments.selector;
                        var type = shortcut.arguments.type;
                        Mousetrap.bind(sequences, (function(selector, type, prevent_default) {
                            var callback = function(e, combo) {
                                $(selector).trigger(type);
                                return !prevent_default;
                            };
                            return callback;
                        })(selector, type, prevent_default));
                        break;
                    case 'redirect':
                        var url = shortcut.arguments.url;
                        Mousetrap.bind(sequences, (function(url, prevent_default) {
                            var callback = function(e, combo) {
                                window.location = url;
                                return !prevent_default;
                            };
                            return callback;
                        })(url, prevent_default));
                        break;
                    case 'callback':
                        var callback_function = shortcut.arguments.callback;
                        Mousetrap.bind(sequences, (function(callback_function, prevent_default){
                            var callback = function(e, combo) {
                              prevent_default &= window[callback_function](e, combo);
                              return !prevent_default;
                            };
                            return callback;
                        })(callback_function, prevent_default));
                        break;
                    }

                }

                Mousetrap['stopCallback'] = function(e, element, combo) {
                    // If the element has the class "mousetrap" then no need to stop.
                    if ((' ' + element.className + ' ').indexOf(' mousetrap ') > -1) {
                        return false;
                    }

                    for(index in settings.keycuts.shortcuts) {
                        var shortcut = settings.keycuts.shortcuts[index];
                        var sequences = shortcut.sequence.split(/[\s,]+/);
                        for(s in sequences){
                            var sequence = sequences[s];
                            if(combo==sequence && shortcut.enable_during_input){
                                return false;
                            }
                        }
                    }

                    // Stop for input, select, and textarea.
                    return element.tagName == 'INPUT' || element.tagName == 'SELECT' || element.tagName == 'TEXTAREA' || (element.contentEditable && element.contentEditable == 'true');
                };
            }
        },

    };

})(jQuery, window, document);
