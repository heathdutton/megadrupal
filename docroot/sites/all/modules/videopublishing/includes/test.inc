<?php
/**
 * @file
 * Simple test code for the Videopublishing module.
 */

/**
 * Provides test and usage examples for the VideopublishingApi.
 */
function videopublishing_test() {
  $vp = VideopublishingApi::getInstance();
  $video_id = '59dc073b1d07a13a8d542e1ec69d64af';
  $playlist_id = '66913fe36c12d13238f217b9d047f019';
  $player_id = 'b4fe538e73ae1a41613ac300488cf977';
  $format_id = 2;
  $publish_id = 'f8249c6d328235bd8758bd4d856d6262';

  //$p = $vp->getPlayers(); dpm($p);
  $v = $vp->getVideos(); dpm(json_decode($v));
  $v = $vp->getVideos(Api_Methods_Factory::XML_RESPONSE); dpm($v);

  //$s = $vp->getStreams($video_id); dpm($s);

  //$v = $vp->getPlaylistVideos($playlist_id); dpm($v);

  //$ec = $vp->getStreamEmbedCode($video_id, $player_id, 1); dpm($ec);

  //$f = $vp->getFormats(); dpm($f);

  //$path = 'D:\temp\video\16c9b36ca9c9d1a9631a3785105d08cb.asf';
  //$name = 'Test upload 04.04.2012 - 13:31';
  //$u = $vp->uploadVideo($path, $name); dpm($u);

  //$i = $vp->getVideoInfo($video_id); dpm($i);

  //$s = $vp->setStream($video_id, $format_id); dpm($s);

  //$stream_id = $video_id . '_' . $format_id;
  //$ps = $vp->publish($player_id, $stream_id, 'stream'); dpm($ps);

  //$pp = $vp->publish($player_id, $playlist_id, 'playlist'); dpm($pp);

  //$pec = $vp->getPublishEmbedCode($publish_id); dpm($pec);
}