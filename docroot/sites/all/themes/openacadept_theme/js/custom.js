jQuery(document).ready(function(){
    jQuery('#section-header #branding-2 #search-block-form input.form-submit').val('');
    jQuery('#edit-search-block-form--2').focus(function(){
        jQuery(this).animate(function(){
            jQuery(this).css({
                'width': 500
            });
        });
    });
});

(function($){
    $(document).ready(function() {
        
        $("#superfish ul.menu li").removeClass('sfHover');
        $("#superfish ul.menu").addClass('sf-menu');
        $("#superfish ul.menu").addClass('sf-navbar');
        $("#superfish ul.menu").superfish({
            pathClass:  'active-trail',
            speed:      1,
            delay:      2000,
            pathLevels: 1,
            dropShadows:false,
            disableHI:  true
        });
    });
})(jQuery);
