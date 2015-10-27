<?php
/**
 * @file
 * Remove files with badly encoded names.
 */

/**
 * Recursive helper to process the images in a folder.
 *
 * We are just collecting all the files that match our regex and have a weird
 * encoding.
 */
function get_badly_encoded_files($dir) {
  $bad_files = array();
  $files = scandir($dir);
  $i = 0;
  foreach ($files as $file) {
    $i++;
    $full_file = "{$dir}/{$file}";
    if (is_dir($full_file) && ($file != "." && $file != "..")) {
      $bad_files = array_merge($bad_files, get_badly_encoded_files($full_file));
    }
    elseif (!is_dir($full_file)) {
      $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
      if (preg_match($regex, $full_file)) {
        $enc = mb_detect_encoding($full_file);
        if ($enc != 'ASCII') {
          print_r("{$full_file}\n");
          $bad_files[] = $full_file;
        }
      }
    }
  }
  print_r("Scanned $i files.\n");
  return $bad_files;
}

// The only argument needed is the directory to process without a / at the end.
if (!empty($argv[1]) && is_dir($argv[1])) {
  $dir = $argv[1];
}
else {
  throw new Exception("Need a directory as the first argument for this script");
}
$bad_files = get_badly_encoded_files($dir);

$done = FALSE;

$ask = TRUE;
if (!empty($argv[2]) && $argv[2] == "-y") {
  $ask = FALSE;
}

while (!$done) {
  $delete = "";

  if ($ask) {
    $delete = readline("Do you want to delete these files? (Y/N)");
  }
  else {
    print_r("Fine! Won't ask if you want to delete them.\n");
  }

  if ($delete == "Y" || !$ask) {
    foreach ($bad_files as $file) {
      unlink($file);
      print_r("Deleted {$file}\n");
    }
    $done = TRUE;
  }
  elseif ($delete == "N") {
    $done = TRUE;
    print_r("I understand, deleting stuff is dangerous!!\n");
  }
  else {
    print_r("Invalid option. Try again..\n");
  }
}
