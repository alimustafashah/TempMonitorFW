# TempMonitorFW
<h1>Monitor de Temperatura Automotivo</h1>
<p>O c&oacute;digo faz parte de um projeto de monitor de temperatura automotivo com Ardu&iacute;no nano. O principal objetivo &eacute; controlar a velocidade da ventoinha do carro de acordo com a temperatura da &aacute;gua, mais destalhes a seguir:</p>
<h2>Componentes:</h2>
<p>Esses s&atilde;o os componentes utilizados durante o desenvolvimento:</p>
<ol>
    <li>Ardu&iacute;no Nano</li>
    <li>Display LCD 16x2 (1602A)</li>
    <li>1 Resistor</li>
    <li>1 Resistor 4.75k</li>
</ol>
<p>Esses s&atilde;o os componentes utilizados durante os testes no Tinkercad:</p>
<ol>
    <li>Ardu&iacute;no Uno R3</li>
    <li>Placa de ensaio pequena</li>
    <li>Display LCD 16x2</li>
    <li>Sensor de temperatura (TMP36)</li>
    <li>Resistor 1k</li>
    <li>Regulador 5v (LM7805)</li>
    <li>Regulador 3.3v (LD1117V33)</li>
    <li>Interruptor deslizante</li>
    <li>8 Baterias AA 1.5v (total: 12v)</li>
</ol>
<h2>Funcionamento do c&oacute;digo:</h2>
<p>*Pinos comentados no topo do c&oacute;digo</p>
<ol>
    <li>energizar</li>
    <li>apresenta&ccedil;&atilde;o no display</li>
    <li>Dados sobre temperatura/velocidade (como na imagem a seguir...)</li>
</ol>
<section><em>[Imagem em breve...]</em></section>
<p>1-3 pinos ir&atilde;o liberar 5v em certas temperaturas, o ultimo alimentara um piezo.</p>
<p>O c&oacute;digo tamb&eacute;m pode ser modificado para uso de PWM</p>
