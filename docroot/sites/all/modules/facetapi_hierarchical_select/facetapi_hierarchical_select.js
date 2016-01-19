(function ($) {
    Drupal.behaviors.facetapiHierarchicalSelect = {
        attach: function (context) {
            $('.facetapi-hierarchical-select').change(function () {
                var form = $(this).closest('form');
                //change the input value used in form submit handler.
                form.find("input[name='facetapi_hierarchical_select_selected_select']").val($(this).attr('name'));
                form.submit();
            });
        }
    };
})(jQuery);
