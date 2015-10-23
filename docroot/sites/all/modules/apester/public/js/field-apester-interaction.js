(function($, window, undefined) {
    'use strict';

    var document = window.document,
        Drupal = window.Drupal,
        map = {
            addInteraction: addInteraction,
            editInteraction: editInteraction,
            chooseInteraction: chooseInteraction,
            interactionCreated: closeAndEmbed
        },
        editElm;

    function init() {
        editElm = $('.edit-apester-field-interaction');
    }

    function getSuggestionsFrame() {
        return document.getElementById('apester_meta_box_suggestions');
    }

    function notify() {
        var contentWindow = getSuggestionsFrame().contentWindow;

        if (!contentWindow) {
            return;
        }

        contentWindow.postMessage({ action: 'interactionCreated' }, '*');
    }

    function closeAndEmbed(id) {
        modal.close();
        editElm.val(id);
        notify();
    }

    function chooseInteraction(id) {
        editElm.val(id);
    }

    function editInteraction(id) {
        modal.open(Drupal.settings.apester.editorUrl + '/#/editor/' + id + '?access_token=' + Drupal.settings.apester.authToken);
    }

    function addInteraction() {
        modal.open(Drupal.settings.apester.editorUrl + '/#/editor/new' + '?access_token=' + Drupal.settings.apester.authToken);
    }

    function messageHandler(event) {
        var data = event.data,
            params = data.params || null;

        if (!data.action || !map[data.action]) {
            return;
        }

        map[data.action](data.id);
    }

    $(document).ready(init);
    window.addEventListener('message', messageHandler, false);
}(jQuery, window));