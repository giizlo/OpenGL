#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec2 resolution;
uniform float time;
uniform float now;

#define PI 3.141592653589793
#define TWO_PI 6.283185307179586




float circle(vec2 uv, vec2 center, float radius, float smoothness) {
    return smoothstep(radius + smoothness, radius - smoothness, length(uv - center));
}

float line(vec2 uv, vec2 center, float angle, float length, float width) {
    vec2 dir = vec2(sin(angle), cos(angle));
    vec2 perp = vec2(dir.y, -dir.x);
    vec2 relPos = uv - center;
    float proj = dot(relPos, dir);
    float dist = dot(relPos, perp);
    return (proj >= 0.0 && proj <= length) ? smoothstep(width, 0.0, abs(dist)) : 0.0;
}




void main() {
    vec2 uv = TexCoord * resolution;
    vec2 center = resolution * 0.5;
    float radius = min(resolution.x, resolution.y) * 0.2;  

    // Цвета элементов 
    vec3 hourHandColor = vec3(0.0, 0.45, 0.9);    
    vec3 minuteHandColor = vec3(0.3, 0.3, 0.3);   
    vec3 secondHandColor = vec3(0.7, 0.7, 0.7);   
    vec3 nowHandColor = vec3(1.0, 1.0, 1.0);      
    vec3 clockColor = vec3(0.9);                   

    // Расчёт времени
    float seconds = mod(time, 60.0);
    float minutes = mod(time / 60.0, 60.0);
    float hours = mod(time / 3600.0, 12.0);
    float nowTime = mod(now, 60.0);

    // Углы стрелок (в радианах)
    float secondAngle = TWO_PI * (seconds / 60.0);
    float minuteAngle = TWO_PI * (minutes / 60.0);
    float hourAngle = TWO_PI * (hours / 12.0);
    float nowAngle = TWO_PI * (nowTime / 60.0);

    // Инициализация прозрачного фона
    FragColor = vec4(0.0, 0.0, 0.0, 0.0);

    
    // 1. Циферблат 
    float outer = circle(uv, center, radius, 2.0);
    float inner = circle(uv, center, radius * 0.9, 2.0);
    float ring = outer - inner;
    FragColor = mix(FragColor, vec4(clockColor, 1.0), ring);

    // 2. Стрелки
    float hourHand = line(uv, center, hourAngle, radius * 0.5, 5.0);
    float minuteHand = line(uv, center, minuteAngle, radius * 0.7, 3.5);
    float secondHand = line(uv, center, secondAngle, radius * 0.8, 2.0);
    float nowHand = line(uv, center, nowAngle, radius * 0.8, 2.0);

    FragColor = mix(FragColor, vec4(hourHandColor, 1.0), hourHand);
    FragColor = mix(FragColor, vec4(minuteHandColor, 1.0), minuteHand);
    FragColor = mix(FragColor, vec4(secondHandColor, 1.0), secondHand);
    FragColor = mix(FragColor, vec4(nowHandColor, 1.0), nowHand);

    // 3. Центральная точка
    float dot = circle(uv, center, 4.0, 1.0);
    FragColor = mix(FragColor, vec4(0.0, 0.0, 0.0, 1.0), dot);
}