<?php

// Portuguese Stopwords
function autordf_stopwords_pt() {
  $sp = 'último, acerca, agora, algmas, alguns, ali, ambos, antes, apontar,' .
      ' aquela, aquelas, aquele, aqueles, aqui, atrás, bem, bom, cada, caminho,' .
      ' cima, com, como, comprido, conhecido, corrente, das, debaixo, dentro,' .
      ' desde, desligado, deve, devem, deverá, direita, diz, dizer, dois, dos,' .
      ' ela, ele, eles, enquanto, então, está, estão, estado, estar, estará,' .
      ' este, estes, esteve, estive, estivemos, estiveram, fará, faz, fazer,' .
      ' fazia, fez, fim, foi, fora, horas, iniciar, inicio, irá, ista, iste,' .
      ' isto, ligado, maioria, maiorias, mais, mas, mesmo, meu, muito, muitos,' .
      ' nós, não, nome, nosso, novo, onde, outro, para, parte, pegar, pelo,' .
      ' pessoas, pode, poderá, podia, por, porque, povo, promeiro, quê, qual,' .
      ' qualquer, quando, quem, quieto, são, saber, sem, ser, seu, somente,' .
      ' têm, tal, também, tem, tempo, tenho, tentar, tentaram, tente, tentei,' .
      ' teu, teve, tipo, tive, todos, trabalhar, trabalho, uma, umas, uns, usa,' .
      ' usar, valor, veja, ver, verdade, verdadeiro, você';
  
  $list = explode(', ', $sp);
  return array('pt' => $list); 
}