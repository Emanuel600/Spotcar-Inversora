# Spotcar Inversora
Projeto TCC, constituindo uma repuxadeira elétrica. Planilha de cálculo no arquivo Planilha_de_Cálculo.pynb; separado nas seguintes partes:
 - "Kicad - Projects" - placas que serão construídas, inclui versões alternativas que não serão utilizadas
 - "Spotcar Codebase" - código do microcontrolador escolhido

## Visão Geral
 - Visa principalmente reduzir volume, peso e custo do produto atualmente no mercado (50x40x40cm e 19,5 kg)
 - Deve ser capaz de realizar solda por resistência através da ponteira de solda (mínimo 10A, máximo ~200A)
 - Utilizada para desamassar peças metálicas de carros - não recomendada para lataria de alumínio

## Especificações Técnicas do Sistema
 - Tensão de entrada 220 V @ 60 Hz
 - Potência nominal de saída 2 kVA
 - Corrente máxima de saída 200 A
 - Frequência da corrente de saída 40 kHz
 - Tensão RMS de saída em vazio 10 V (onda quadrada)
 - Tecnologia inversora (Dada a potência, provável o uso de IGBTs)
 - Proteção de sobrecorrente, sobretemperatura e possívelmente subtensão na entrada

## Topologia
Devido à potência relativamente baixa e para reduzir a complexidade do comando - foi escolhida a topologia meia ponte.
