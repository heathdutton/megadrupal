var LoginRadius_Social_JS;

if (window.LoginRadius_Social_JS != true) {
    LoginRadius_Social_JS = true;
    var options = {};
    options.login = true;
    LoginRadius_SocialLogin.util.ready(function () {
        var loc = Drupal.settings.lrsociallogin.callback;
        $ui = LoginRadius_SocialLogin.lr_login_settings;
        $ui.interfacesize = Drupal.settings.lrsociallogin.interfacesize;
        $ui.lrinterfacebackground = Drupal.settings.lrsociallogin.lrinterfacebackground;
        $ui.noofcolumns = Drupal.settings.lrsociallogin.noofcolumns;
        $ui.apikey = Drupal.settings.lrsociallogin.apikey;
        $ui.is_access_token = true;
        if (detectmob()) {
            $ui.isParentWindowLogin = true;
            loc = Drupal.settings.lrsociallogin.location;
        }
        $ui.callback = loc;
        $ui.lrinterfacecontainer = "interfacecontainerdiv";
        LoginRadius_SocialLogin.init(options);
    });
}
if(window.LoginRadiusSDK){
LoginRadiusSDK.setLoginCallback(function () {
    var token = LoginRadiusSDK.getToken();
    var form = document.createElement('form');
    form.action = Drupal.settings.lrsociallogin.location;
    form.method = 'POST';

    var hiddenToken = document.createElement('input');
    hiddenToken.type = 'hidden';
    hiddenToken.value = token;
    hiddenToken.name = 'token';
    form.appendChild(hiddenToken);

    document.body.appendChild(form);
    form.submit();
});
}
function detectmob() {
    if (navigator.userAgent.match(/Android/i) || navigator.userAgent
        .match(/webOS/i) || navigator.userAgent.match(/iPhone/i) ||
        navigator.userAgent.match(/iPad/i) || navigator.userAgent
        .match(/iPod/i) || navigator.userAgent.match(
        /BlackBerry/i) || navigator.userAgent.match(
        /Windows Phone/i)) {
        return true;
    } else {
        return false;
    }
}