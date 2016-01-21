var script = document.createElement('script');
script.type = 'text/javascript';
script.text = 'var islrsharing = true; var islrsocialcounter = true;';
document.body.appendChild(script);

var script_new = document.createElement('script');
script_new.type = 'text/javascript';
script_new.src = '//sharecdn.social9.com/v1/social9.min.js';
document.body.appendChild(script_new);

window.onload = function () {
    if (typeof LoginRadius != 'undefined') {
        if (Drupal.settings.advancelrsocialshare != undefined) {
            if (Drupal.settings.advancelrsocialshare.sharing != undefined && Drupal.settings.advancelrsocialshare.sharing) {
                LoginRadius.util.ready(function () {
                    var lr_interface = Drupal.settings.advancelrsocialshare.lr_interface;
                    $i = $SS.Interface[lr_interface];
                    var string = Drupal.settings.advancelrsocialshare.providers;
                    var providers = string.split(',');
                    $SS.Providers.Top = providers;
                    $u = LoginRadius.user_settings;
                    if (Drupal.settings.advancelrsocialshare.apikey) {
                        $u.apikey = Drupal.settings.advancelrsocialshare.apikey;
                    }
                    $u.sharecounttype = 'url';
                    $i.size = Drupal.settings.advancelrsocialshare.size;
                    $i.top = '0px';
                    $i.left = '0px';
                    if (typeof document.getElementsByName("viewport")[0] != "undefined") {
                        $u.isMobileFriendly = true;
                    }
                    ;
                    $i.show(Drupal.settings.advancelrsocialshare.divwidget);
                });
            }
            if (Drupal.settings.advancelrsocialshare.counter != undefined && Drupal.settings.advancelrsocialshare.counter) {
                LoginRadius.util.ready(function () {
                    var string = Drupal.settings.advancelrsocialshare.providers;
                    var providers = string.split(',');
                    var lr_interface = Drupal.settings.advancelrsocialshare.lr_interface;
                    $SC.Providers.Selected = providers;
                    $S = $SC.Interface[lr_interface];
                    $S.isHorizontal = true;
                    $S.countertype = Drupal.settings.advancelrsocialshare.countertype;
                    $u = LoginRadius.user_settings;
                    if (typeof document.getElementsByName("viewport")[0] != "undefined") {
                        $u.isMobileFriendly = true;
                    }
                    ;
                    $S.show(Drupal.settings.advancelrsocialshare.divwidget);
                });
            }
            if (Drupal.settings.advancelrsocialshare.verticalsharing != undefined && Drupal.settings.advancelrsocialshare.verticalsharing) {
                LoginRadius.util.ready(function () {
                    var lr_interface = Drupal.settings.advancelrsocialshare.lr_vertical_interface;
                    $i = $SS.Interface[lr_interface];
                    var string = Drupal.settings.advancelrsocialshare.vertical_providers;
                    var providers = string.split(',');
                    $SS.Providers.Top = providers;
                    $u = LoginRadius.user_settings;
                    if (Drupal.settings.advancelrsocialshare.vertical_apikey) {
                        $u.apikey = Drupal.settings.advancelrsocialshare.vertical_apikey;
                    }
                    $u.sharecounttype = 'url';
                    $i.size = Drupal.settings.advancelrsocialshare.vertical_size;
                    if (Drupal.settings.advancelrsocialshare.vertical_offset && Drupal.settings.advancelrsocialshare.vertical_offset != '0px') {
                        $i.top = Drupal.settings.advancelrsocialshare.vertical_offset;
                    }
                    else {
                        $i[Drupal.settings.advancelrsocialshare.vertical_position1] = '0px';
                    }
                    $i[Drupal.settings.advancelrsocialshare.vertical_position2] = '0px';
                    if (typeof document.getElementsByName("viewport")[0] != "undefined") {
                        $u.isMobileFriendly = true;
                    }
                    ;
                    $i.show(Drupal.settings.advancelrsocialshare.vertical_divwidget);
                });
            }
            if (Drupal.settings.advancelrsocialshare.verticalcounter != undefined && Drupal.settings.advancelrsocialshare.verticalcounter) {
                LoginRadius.util.ready(function () {
                    var string = Drupal.settings.advancelrsocialshare.vertical_providers;
                    var providers = string.split(',');
                    $SC.Providers.Selected = providers;
                    $S = $SC.Interface.simple;
                    $S.isHorizontal = false;
                    $S.countertype = Drupal.settings.advancelrsocialshare.vertical_countertype;
                    if (Drupal.settings.advancelrsocialshare.vertical_offset && Drupal.settings.advancelrsocialshare.vertical_offset != '0px') {
                        $S.top = Drupal.settings.advancelrsocialshare.vertical_offset;
                    }
                    else {
                        $S[Drupal.settings.advancelrsocialshare.vertical_position1] = '0px';
                    }

                    $S[Drupal.settings.advancelrsocialshare.vertical_position2] = '0px';
                    $u = LoginRadius.user_settings;
                    if (typeof document.getElementsByName("viewport")[0] != "undefined") {
                        $u.isMobileFriendly = true;
                    }
                    ;
                    $S.show(Drupal.settings.advancelrsocialshare.vertical_divwidget);
                });
            }
        }
    }
}