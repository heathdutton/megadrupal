function redirect(token, name) {
    var token_name = name ? name : 'token';
    jQuery('#fade').show();
    var form = document.createElement('form');
    form.action = LocalDomain;
    form.method = 'POST';

    var hiddenToken = document.createElement('input');
    hiddenToken.type = 'hidden';
    hiddenToken.value = token;
    hiddenToken.name = token_name;
    form.appendChild(hiddenToken);
    if ((window.lr_source) && (lr_source == 'wall_post' || lr_source == 'friend_invite')) {
        var hiddenToken = document.createElement('input');
        hiddenToken.type = 'hidden';
        hiddenToken.value = lr_source;
        hiddenToken.name = 'lr_source';
        form.appendChild(hiddenToken);
    }
    document.body.appendChild(form);
    form.submit();
}