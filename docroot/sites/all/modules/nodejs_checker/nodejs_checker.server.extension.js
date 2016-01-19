/**
 * @File
 * Nodejs Check server extension for nodejs
 *
 * IMPORTANT ! Add this extension name into node.config.js
 */

var configuration;

var stopper = {
  messageType : 'nodejs-stopper',
}

var starter = {
  messageType : 'nodejs-starter',
}

process.on('uncaughtException', function(err) {
  console.log(err);
  configuration.sendMessageToBackend(stopper, exit);
});

process.on('exit', function() {
  configuration.sendMessageToBackend(stopper, exit);
});

process.on('SIGINT', function() {
  configuration.sendMessageToBackend(stopper, exit);
});

process.on('SIGABRT', function() {
  configuration.sendMessageToBackend(stopper, exit);
});

process.on('SIGHUP', function() {
  configuration.sendMessageToBackend(stopper, exit);
});


function exit() {
  process.exit(1);
}

exports.setup = function(config) {
  configuration = config;
  configuration.sendMessageToBackend(starter);
}
