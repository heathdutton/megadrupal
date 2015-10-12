(function($){ // Create local scope.

Drupal.ajax.prototype.commands.brreg_populate = function (ajax, response, status) {

    // Remove any previously added error messages.
    var $element = $(ajax.element);
    $element.parent().find('.messages').remove();

    // Populate fields.
    $.each(response.data, function(index){
        $(':[data-brregkey="' + index + '"]', ajax.element.form).val($(this).get());
    });

    // Show error message.
    if (response.error) {
        var $messages = $('<div class="messages error"></div>').text(response.error);
        $element.parent().prepend($messages);
    }
};

})(jQuery);