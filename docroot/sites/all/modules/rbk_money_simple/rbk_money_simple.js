jQuery(document).ready(function () {

    // select fields on focus
    jQuery("#edit-rbk-money-simple-response, #edit-description").focus(function() {
        var $this = jQuery(this);
        $this.select();

        // Work around Chrome's little problem
        $this.mouseup(function() {
            // Prevent further mouseup intervention
            $this.unbind("mouseup");
            return false;
        });
    });


    // count description symbols
    var desc = jQuery('#edit-description');
    var hint = desc.parent().siblings('.description');
    var descLength;

    desc.keyup(function() {
        descLength = ((255 - desc.val().length) > 0) ? (255 - desc.val().length) : 0;
        hint.text('Available ' + descLength + ' symbols');
    })


});