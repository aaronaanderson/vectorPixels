
#version 430


//trying to recreate this:https://youtu.be/ICrNOTQBS8U at 3:45
uniform int maxIterations = 300;
uniform float maxSpeed = 15;//speed in pixels per second
struct Magnet{
    vec2 location;
    vec2 velocity;
    vec3 color;
    float size;
};

layout(std430, binding = 0) buffer bufferObject{
    Magnet magnets[];
};

vec2 limitVector(vec2 originalVector, float limit){
    float originalLength = length(originalVector);
    if(originalLength > limit){
        float adjustment = limit/originalLength;
        originalVector *= adjustment;
        return originalVector;//'originalVector' is now adjusted
    }
    //if the above condition fails
    return originalVector;
}

out vec4 outputColor;

vec2 seekerLocation;
vec2 seekerVelocity = vec2(0.0, 0.0);
vec2 desiredVelocity;
vec2 seekerPreviousVelocity = vec2(0.0, 0.0);
bool collidedWithMagnet = false;
int magnetCollisionIndex = -1;//keep track of which magnet was collided with 



void main(){

    seekerLocation = vec2(gl_FragCoord.x, gl_FragCoord.y);//start the location at the pixel location
    
    //input test
    // float minDist = 100000.0;
    // for(int i = 0; i < magnets.length(); i++){
    //     float dist = distance(seekerLocation, magnets[i].location);
    //     if( dist < minDist){
    //         magnetCollisionIndex = i;
    //         minDist = dist;
    //     }
    // }
    
    for(int i = 0; i < maxIterations; i++){
        seekerLocation += seekerVelocity;//apply the velocity to the location
        
        //check for collision with magnet
        for(int i = 0; i < magnets.length(); i++){
            if(distance(magnets[i].location, seekerLocation) < magnets[i].size){
                magnetCollisionIndex = i;
                collidedWithMagnet = true;
                break;
            }
        }
        if(collidedWithMagnet)
            break;

        //update velocity
        for(int i = 0; i < magnets.length(); i++){
            desiredVelocity += (((magnets[i].location - seekerLocation)*magnets[i].size) / 
            pow((distance(magnets[i].location, seekerLocation)+1), 2));
        }
        //seekerVelocity = desiredVelocity - seekerPreviousVelocity;
        seekerVelocity = limitVector(desiredVelocity, maxSpeed);
        seekerPreviousVelocity = seekerVelocity;
        //seekerVelocity = max(seekerVelocity, seekerPreviousVelocity)
        //velocity will be updated one extra time than it needs to TODO fix this

    }

    vec4 c = vec4(0.0, 0.0, 0.0, 1.0);
    if(magnetCollisionIndex > -1){
      c = vec4(magnets[magnetCollisionIndex].color, 1.0);
    }
    outputColor = c;
}