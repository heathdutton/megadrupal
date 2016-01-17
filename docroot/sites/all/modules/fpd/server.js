/**
 * Provides a simple file serving mechanism.
 *
 * Authentication is passed off to a backend via http.
 */

var express = require('express'),
    request = require('request'),
    fs = require('fs'),
    vm = require('vm'),
    server = express.createServer();

try {
  var settings = vm.runInThisContext(fs.readFileSync(process.cwd() + '/fpd.config.js'));
}
catch (exception) {
  console.log("Failed to read config file, exiting: " + exception);
  process.exit(1);
}

/**
 * Returns the backend url.
 */
var getBackendUrl = function (fpdToken, fpdFid) {
  return settings.backend.scheme + '://' + settings.backend.host + ':' + settings.backend.port +
       '/' + settings.backend.fpdPath + '/' + fpdToken + '/' + fpdFid;
}

server.get('/fpd/server/:fpd_token/:fpd_fid', function (req, res) {
  var fpdToken = req.params.fpd_token || '';
  var fpdFid = req.params.fpd_fid || '';
  var auth = {};
  request(getBackendUrl(fpdToken, fpdFid), function (error, backend_res, body) {
    if (!error && backend_res.statusCode == 200) {
      try {
        auth = JSON.parse(body);
      }
      catch (exception) {
        auth.access = false;
      }
      if (auth.access) {
        for (var header in auth.headers) {
          res.setHeader(header, auth.headers[header]);
        }
        res.download(auth.realpath, auth.filename);
      }
      else {
        res.send(403);
      }
    }
  });
});

server.listen(settings.port, settings.host);

// vi:ai:expandtab:sw=2 ts=2

