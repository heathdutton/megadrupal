use POSIX;

$type = shift;
$display = shift;

if (!$type || $type ne 'merged') {
  $type = 'distinct';
}

if (!$display || $display ne 'minimal') {
  $display = 'normal';
}

open(LOG, '/home/kiru/download/xc-1.0rc-test/apache-solr-3.3.0/example/drupal_test.cache.' . $type . '.log') || die "Can't open input file $!\n";

%analitics = ();
while (<LOG>) {
  if (/^INFO: \[wordnet\]/) {
    next;
  }

  $print = 0;
  $qtime = 0;
  $xcname = '';

  if (/QTime=(\d+)/) {
    $qtime = $1;
    $print = 1;
  }

  if (/ hits=(\d+)/) {
    $hits = $1;
    $print = 1;
  }

  if (/XCNAME=(\w+)/) {
    $xcname = $1;
    $print = 1;
  }

  if (/&q=([^\&\}]+)/) {
    $q = $1;
    $print = 1;
  }

  if ($print == 1) {
    if ($xcname ne '' && $q ne 'dummy') {
      $analitics{lc($q)}{'hits'} = $hits;
      push(@{$analitics{lc($q)}{$xcname}}, $qtime);
    }
  }
}
close LOG;

# xc_search_do_search, xc_search_block_facets
foreach $q (sort keys %analitics) {
  if ($type eq 'distinct') {
    printf("%s (%d)\n", $q, $analitics{$q}{'hits'}) if ($display eq 'normal');

    @tss = (); $min_ts = $max_ts = $total_ts = 0;
    @tfs = (); $min_tf = $max_tf = $total_tf = 0;
    @totals = (); $total = 0;

    for ($i=0; $i <= $#{$analitics{$q}{'xc_search_do_search'}}; $i++) {
      $ts = ${$analitics{$q}{'xc_search_do_search'}}[$i];
      push(@tss, $ts);
      $total_ts += $ts;

      $tf = ${$analitics{$q}{'xc_search_block_facets'}}[$i];
      push(@tfs, $tf);
      $total_tf += $tf;

      if ($i == 0) {
        $min_ts = $max_ts = $ts;
        $min_tf = $max_tf = $tf;
      }
      else {
        $min_ts = $ts if ($min_ts > $ts);
        $min_tf = $tf if ($min_tf > $tf);
        $max_ts = $ts if ($max_ts < $ts);
        $max_tf = $tf if ($max_tf < $tf);
      }
      $subtotal = $ts + $tf;
      push(@totals, $subtotal);
      $total += $subtotal;
    }
    $avarage = $total_ts / scalar(@tss);
    $perc = $max_ts / 10;
    %percs = ();
    foreach $i (@tss) {
      $percs{ceil($i / $perc)}++;
    }
    printf("\tsearch\t(%d-%d / avg: %d): ", $min_ts, $max_ts, $avarage) if ($display eq 'normal');
    @items = ();
    foreach $i (sort keys %percs) {
      push(@items, sprintf("%d in %dth", $percs{$i}, $i));
    }
    printf("clusters: %s (raw numbers: %s)\n", join(', ', @items), join(', ', @tss)) if ($display eq 'normal');

    $avarage = $total_tf / scalar(@tfs);
    $perc = $max_tf / 10;
    %percs = ();
    foreach $i (@tfs) {
      $percs{ceil($i / $perc)}++;
    }
    printf("\tfacet\t(%d-%d / avg: %d): ", $min_tf, $max_tf, $avarage) if ($display eq 'normal');
    @items = ();
    foreach $i (sort {$a <=> $b} keys %percs) {
      push(@items, sprintf("%d in %dth", $percs{$i}, $i));
    }
    printf("clusters: %s (raw numbers: %s)\n", join(', ', @items), join(', ', @tfs)) if ($display eq 'normal');

    $avarage = $total / scalar(@totals);
    printf("\ttotal\t%d (avg: %d)\n", $total, $avarage) if ($display eq 'normal');
    printf("%s\t%d\n", $q, $avarage) if ($display eq 'minimal');
  }
  elsif ($type eq 'merged') {
    printf("%s (%d)\n", $q, $analitics{$q}{'hits'}) if ($display eq 'normal');

    @tss = (); $min_ts = 0; $max_ts = 0;

    $ts_total = 0;
    for ($i=0; $i <= $#{$analitics{$q}{'xc_search_do_search_and_facet'}}; $i++) {
      $ts = ${$analitics{$q}{'xc_search_do_search_and_facet'}}[$i];
      push(@tss, $ts);
      $ts_total += $ts;
      if ($i == 0) {
        $min_ts = $max_ts = $ts;
      }
      else {
        $min_ts = $ts if ($min_ts > $ts);
        $max_ts = $ts if ($max_ts < $ts);
      }
    }
    $avarage = $ts_total / scalar(@tss);

    $perc = $max_ts / 10;
    %percs = ();
    foreach $i (@tss) {
      $percs{ceil($i / $perc)}++;
    }
    printf("\tsearch\t(%d-%d / avg: %d): ", $min_ts, $max_ts, $avarage) if ($display eq 'normal');
    @items = ();
    foreach $i (sort keys %percs) {
      push(@items, sprintf("%d in %dth", $percs{$i}, $i));
    }
    printf("clusters: %s (raw numbers: %s)\n", join(', ', @items), join(', ', @tss)) if ($display eq 'normal');
    printf("%s\t%d\n", $q, $avarage) if ($display eq 'minimal');
  }
}
