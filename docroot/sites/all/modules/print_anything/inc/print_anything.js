(function ($) {
    Drupal.behaviors.printAnyhingBehavior = {
        attach: function (context, settings) {
            $(document).ready(function() {
                $('#print-anything-link a', context).click(function () {
                    genPopUp = window.open("", "printer", "toolbar=no,location=no,scrollbars=yes,directories=no,status=yes,menubar=yes,resizable=yes,width=760,height=600");
                    genPopUp.location.href = Drupal.settings.print_anything.print_page_url;
                    if (genPopUp.opener == null) genPopUp.opener = window;
                    genPopUp.opener.name = "opener";
                });
                $('#printLink a').click(function () {
                    window.print();
                    return false;
                });
            });
        }
    };
})(jQuery);