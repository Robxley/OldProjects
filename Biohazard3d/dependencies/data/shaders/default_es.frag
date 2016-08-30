


// Précision pour les variables de type float

precision mediump float;


// Uniform

uniform sampler2D texture0;


// Varying

varying vec2 texCoord;


// Fonction main

void main()
{
    // Couleur du pixel
    
    gl_FragColor = texture2D(texture0, texCoord);
}
