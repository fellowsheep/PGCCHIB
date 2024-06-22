import org.lwjgl.Lwjgl
import org.lwjgl.Lwjgl.implementation
import org.lwjgl.sonatype

plugins {
    id("java")
    id("org.lwjgl.plugin") version "0.0.34"
}

group = "org.example"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
    sonatype()
}

dependencies {
    implementation(Lwjgl.Preset.everything)
    implementation("org.joml:joml:1.10.5")
}