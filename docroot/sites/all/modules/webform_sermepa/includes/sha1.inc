<?php

/**
 * @file
 * SHA class for sermepa connections not "Sha ampliada".
 */

/**
 * WebformSermepaSha1.
 */
class WebformSermepaSha1 {
  private $A;
  private $B;
  private $C;
  private $D;
  private $E;
  private $ta;
  private $tb;
  private $tc;
  private $td;
  private $te;
  private $K019;
  private $K2039;
  private $K4059M;
  private $K6079;

  private $buffer;
  private $buffsize;
  private $totalsize;

  /**
   * Constructor function.
   */
  public function sha() {
    $this->init();
  }

  /**
   * Init function.
   */
  private function init() {
    $this->A = 0x6745 << 16 | 0x2301;
    $this->B = 0xefcd << 16 | 0xab89;
    $this->C = 0x98ba << 16 | 0xdcfe;
    $this->D = 0x1032 << 16 | 0x5476;
    $this->E = 0xc3d2 << 16 | 0xe1f0;
    $this->ta = $this->A;
    $this->tb = $this->B;
    $this->tc = $this->C;
    $this->td = $this->D;
    $this->te = $this->E;
    $this->K019 = 0x5a82 << 16 | 0x7999;
    $this->K2039 = 0x6ed9 << 16 | 0xeba1;
    $this->K4059 = 0x8f1b << 16 | 0xbcdc;
    $this->K6079 = 0xca62 << 16 | 0xc1d6;
    $this->buffer = array();
    $this->buffsize = 0;
    $this->totalsize = 0;
  }

  /**
   * BytesToWords function.
   */
  public function bytesToWords($block) {
    $nblk = array();
    for ($i = 0; $i < 16; ++$i) {
      $index = $i * 4;
      $nblk[$i] = 0;
      $nblk[$i] |= ($block[$index] & 0xff) << 24;
      $nblk[$i] |= ($block[$index + 1] & 0xff) << 16;
      $nblk[$i] |= ($block[$index + 2] & 0xff) << 8;
      $nblk[$i] |= ($block[$index + 3] & 0xff);
    }
    return $nblk;
  }

  /**
   * PadBlock function.
   */
  public function padBlock($block, $size) {
    $bits = $size * 8;

    $newblock = $block;
    $newblock[] = 0x80;
    while ((count($newblock) % 64) != 56) {
      $newblock[] = 0;
    }
    for ($i = 0; $i < 8; ++$i) {
      $newblock[] = ($i < 4) ? 0 : ($bits >> ((7 - $i) * 8)) & 0xff;
    }

    return $newblock;
  }

  /**
   * CircShl function.
   */
  private function circShl($num, $amt) {
    $leftmask = 0xffff | (0xffff << 16);
    $leftmask <<= 32 - $amt;
    $rightmask = 0xffff | (0xffff << 16);
    $rightmask <<= $amt;
    $rightmask = ~$rightmask;

    $remains = $num & $leftmask;
    $remains >>= 32 - $amt;
    $remains &= $rightmask;

    $res = ($num << $amt) | $remains;

    return $res;
  }

  /**
   * F019 function.
   */
  private function f019($x, $y, $z) {
    return ($x & $y) | (~$x & $z);
  }

  /**
   * F2039 function.
   */
  private function f2039($x, $y, $z) {
    return ($x ^ $y ^ $z);
  }

  /**
   * F4059 function.
   */
  private function f4059($x, $y, $z) {
    return ($x & $y) | ($x & $z) | ($y & $z);
  }

  /**
   * F6079 function.
   */
  private function f6079($x, $y, $z) {
    return $this->f2039($x, $y, $z);
  }

  /**
   * ExpandBlock function.
   */
  public function expandBlock($block) {
    $nblk = $block;
    for ($i = 16; $i < 80; ++$i) {
      $nblk[$i] = $this->circShl($nblk[$i - 3] ^ $nblk[$i - 8] ^ $nblk[$i - 14] ^ $nblk[$i - 16], 1);
    }

    return $nblk;
  }

  /**
   * PrintBytes function.
   */
  public function printBytes($bytes) {
    $len = count($bytes);
    for ($i = 0; $i < $len; ++$i) {
      $str[] = sprintf("%02x", $bytes[$i]);
    }

    print (implode(", ", $str) . "\n");
  }

  /**
   * Wordstr function.
   */
  private function wordstr($word) {
    return sprintf(
       "%04x%04x", ($word >> 16) & 0xffff, $word & 0xffff
    );
  }

  /**
   * PrintWords function.
   */
  public function printWords($words) {
    $len = count($words);
    for ($i = 0; $i < $len; ++$i) {
      $str[] = $this->wordstr($words[$i]);
    }

    print (implode(", ", $str) . "\n");
  }

  /**
   * HastToString function.
   */
  public function hastToString($hash) {
    $astr = '';
    $len = count($hash);
    for ($i = 0; $i < $len; ++$i) {
      $astr .= $this->wordstr($hash[$i]);
    }
    return $astr;
  }

  /**
   * Add function.
   */
  private function add($a, $b) {
    $ma = ($a >> 16) & 0xffff;
    $la = ($a) & 0xffff;
    $mb = ($b >> 16) & 0xffff;
    $lb = ($b) & 0xffff;

    $ls = $la + $lb;

    if ($ls > 0xffff) {
      $ma += 1;
      $ls &= 0xffff;
    }

    $ms = $ma + $mb;
    $ms &= 0xffff;

    $result = ($ms << 16) | $ls;
    return $result;
  }

  /**
   * ProcessBlock function.
   */
  private function processBlock($blk) {
    $blk = $this->expandBlock($blk);

    for ($i = 0; $i < 80; ++$i) {
      $temp = $this->circShl($this->ta, 5);
      if ($i < 20) {
        $f = $this->f019($this->tb, $this->tc, $this->td);
        $k = $this->K0_9;
      }
      elseif ($i < 40) {
        $f = $this->f2039($this->tb, $this->tc, $this->td);
        $k = $this->K2039;
      }
      elseif ($i < 60) {
        $f = $this->f4059($this->tb, $this->tc, $this->td);
        $k = $this->K4059;
      }
      else {
        $f = $this->f6079($this->tb, $this->tc, $this->td);
        $k = $this->K6079;
      }

      $temp = $this->add($temp, $f);
      $temp = $this->add($temp, $this->te);
      $temp = $this->add($temp, $blk[$i]);
      $temp = $this->add($temp, $k);

      $this->te = $this->td;
      $this->td = $this->tc;
      $this->tc = $this->circShl($this->tb, 30);
      $this->tb = $this->ta;
      $this->ta = $temp;
    }

    $this->A = $this->add($this->A, $this->ta);
    $this->B = $this->add($this->B, $this->tb);
    $this->C = $this->add($this->C, $this->tc);
    $this->D = $this->add($this->D, $this->td);
    $this->E = $this->add($this->E, $this->te);
  }

  /**
   * Update function.
   */
  public function update($bytes) {
    $length = count($bytes);
    $index = 0;

    while (($length - $index) + $this->buffsize >= 64) {
      for ($i = $this->buffsize; $i < 64; ++$i) {
        $this->buffer[$i] = $bytes[$index + $i - $this->buffsize];
      }
      $this->processBlock($this->bytesToWords($this->buffer));
      $index += 64;
      $this->buffsize = 0;
    }

    $remaining = $length - $index;
    for ($i = 0; $i < $remaining; ++$i) {
      $this->buffer[$this->buffsize + $i] = $bytes[$index + $i];
    }
    $this->buffsize += $remaining;
    $this->totalsize += $length;
  }

  /**
   * Finale function.
   */
  public function finale() {
    for ($i = 0; $i < $this->buffsize; ++$i) {
      $last_block[$i] = $this->buffer[$i];
    }
    $this->buffsize = 0;
    $last_block = $this->padBlock($last_block, $this->totalsize);
    $index = 0;
    $length = count($last_block);
    while ($index < $length) {
      $block = array();
      for ($i = 0; $i < 64; ++$i) {
        $block[$i] = $last_block[$i + $index];
      }
      $this->processBlock($this->bytesToWords($block));
      $index += 64;
    }

    $result[0] = $this->A;
    $result[1] = $this->B;
    $result[2] = $this->C;
    $result[3] = $this->D;
    $result[4] = $this->E;

    return $result;
  }

  /**
   * HashBytes function.
   */
  public function hashBytes($bytes) {
    $this->init();
    $this->update($bytes);
    return $this->finale();
  }

  /**
   * HashString function.
   */
  public function hashString($str) {
    $len = strlen($str);
    for ($i = 0; $i < $len; ++$i) {
      $bytes[] = ord($str[$i]) & 0xff;
    }
    return $this->hashBytes($bytes);
  }

}
