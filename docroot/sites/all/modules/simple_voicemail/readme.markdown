# Simple Voicemail with Tropo

Copyright (c) 2015 Adam Kalsey.

## Introduction

Simple Voicemail is a simple voicemail module for Drupal designed for people who
would like a second phone number that functions as a voicemail box. Use cases
envisioned are a small business that has no fixed line but uses mobile phones,
throwaway numbers for Craigslist ads, or community organizations that want a
voice number backed by nothing but a mailbox.

Messages transcribed and emailed as a WAV file with transcription to you.
[Tropo](http://tropo.com) is used to provide the phone and transcription services.

## Setting up

Upload and enable the simple_voicemail module. See the Drupal documentation for
details on how to install modules.

On your Drupal installation, visit the configuration screen at
`/admin/config/voicemail`. Enter the email address where you'd like messages sent.

Copy the URL from the "Enter this URL as your Application Script" line. You'll
need it in the next step.

Create a Tropo account at http://tropo.com and create a new Tropo
Application. Choose Scripting API for the type. In the form field, instead of
creating a "New Script" or "Select My Files", enter the Tropo Script URL you copied
earlier.

Choose a phone number and click "Create App".

Wait a moment or two for your number to be provisioned and give it a call. Leave
a message and you'll get a voicemail delivered to your email box.

## Configuration

The configuration screen has 3 options.

* *Email messages to*: Where should voicemail messages be sent?

* *Voicemail greeting*: The voicemail greeting. If this is text, will be read out.
If a WAV or MP3 URL, will be played. If blank, the default message plays, saying
that you are not available and asks the caller to leave a message.

* *Before sending to voicemail, forward to*: If set, calls to your number will
be forwarded to this number. Will ring for 15 seconds before voicemail picks
up. Format with the country code as +12125551212. If blank, all calls will go
straight to voicemail.

If `transfer` is set, Tropo requires that your account be enabled for outbound
calling access. Email support@tropo.com to request account verification. The
numbers you can dial are also restricted. See
 https://www.tropo.com/docs/scripting/international-features/international-dialing-sms
