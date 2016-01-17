(function($) {
  // add hook:load. process unzip form
  imce.hooks.load.push (function () {


    imce.unzipOp = {};
    imce.el('edit-unzip').click(function() {
      imce.unzipOpSubmit(fop);
      return false;
    });


    imce.unzipRefreshOp();
    // add hook:navigate. set dirops visibility
    imce.hooks.navigate.push(function (data, olddir, cached) {
      imce.unzipRefreshOp();
    });

  });

  // implementation of imce.hookOpValidate
  imce.unzipOpValidate = function(fop) {
    if (!imce.validateSelCount(1, 1)) return false;
    for (var fid in imce.selected) {
      var row = imce.selected[fid], filename = row.cells[0].innerHTML;

      if (filename.lastIndexOf(".zip") == -1) {
        return imce.setMessage(Drupal.t('The file should be a zip file'), 'error');
      }


    }
    return true ;

  };
  // implementation of imce.hookOpSubmit
  imce.unzipOpSubmit = function(fop) {
    if (imce.fopValidate(fop)) {
      imce.fopLoading(fop, true);
      $.ajax($.extend(imce.fopSettings(fop), {
        success: imce.unzipResponse
        }));
    }

  };
  // change fops states.
  imce.unzipRefreshOp = function () {
    var perm, func = 'opDisable';

    if (perm = imce.conf.perm['unzip']) func = 'opEnable';
    var deleteZip =  $(imce.el('edit-deletezipfile-wrapper'));
    if (imce.conf.perm['delete']) {

      deleteZip.css('display','block');
      deleteZip.find("#edit-deletezipfile").attr('checked', true);
    }
    else {

      deleteZip.css('display','none');

    }
    imce[func]('unzip');
  };

  // Fix browser cache showing the non-cropped image when name is unchanged.
  imce.unzipFids = {};

  imce.unzipGetURL = imce.getURL;
  imce.getURL = function (fid) {
    var url = imce.unzipGetURL(fid);
    // add suffix to prevent caching.
    return imce.unzipFids[fid] ? (url +'?'+ imce.unzipFids[fid]) : url;
  };

  imce.unzipFileGet = imce.fileGet;
  imce.fileGet = function (fid) {
    var file = imce.unzipFileGet(fid);
    if (imce.unzipFids[fid] && file) file.url = file.url.replace(/\?\d+$/, '');
    return file;
  };

  // custom response. keep track of overwritten files.
  imce.unzipResponse = function(response) {

    imce.processResponse(response);
    var set = imce.navSet(imce.conf.dir, 0);

    // load from the cache
    if (imce.conf.dir && imce.cache[imce.conf.dir]) {
      set.success({
        data: imce.cache[imce.conf.dir]
        });
      set.complete();
    }
    // live load
    else $.ajax(set);
    // clean up
    imce.unzipRefreshOp();
  };
})(jQuery);
