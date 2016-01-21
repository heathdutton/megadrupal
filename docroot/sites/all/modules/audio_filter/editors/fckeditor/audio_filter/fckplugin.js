var path = Drupal.settings.audio_filter.url.wysiwyg_fckeditor;
var basePath =  Drupal.settings.basePath;
var modulePath = Drupal.settings.audio_filter.modulepath;

FCKCommands.RegisterCommand('audio_filter', new FCKDialogCommand('audio_filter', '&nbsp;', path, 580, 480));

var oAudioFilterItem = new FCKToolbarButton('audio_filter', 'audio_filter');
oAudioFilterItem.IconPath = basePath + modulePath + '/editors/fckeditor/audio_filter/audio_filter.png';
FCKToolbarItems.RegisterItem('audio_filter', oAudioFilterItem);
