SMS Framework gateway module for any simple HTTP GET/POST gateway interface

To make inbound work you must configure your gateway to send the messages to
URL: http(s)://yourhost.example.com/sms/simplegateway/receiver

The rationale for this module is that many (or most) SMS gateway services use
basic HTTP GET or POST requests, the only difference being the names of the
HTTP parameters. This module allows the user to specify the parameter names
for sending and receiving messages, so that they dont have to write a gateway
module.
