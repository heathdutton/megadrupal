/*
 Adding dynatree settings to the selected menus
 */

(function ($) {
    Drupal.behaviors.drupalDynatree = {
        attach:function (context, settings) {

            //menus to add the script to
            var menus = settings.dynatreeMenus;

            $.each(menus, function (index, value) {
                $(value).dynatree({
                    persist:true,
                    cookie:{
                        path:"/"
                    },
                    cookieId: "dynatree" + index, // Choose a more unique name, to allow multiple trees.

                    onClick:function (node, event) {
                        // Use <a> href and target attributes to load the content:
                        if (node.data.href && node.getEventTargetType(event) == "title") {
                            window.open(node.data.href, '_self');
                        }
                    }

                });
            });


        }
    }
}(jQuery));
