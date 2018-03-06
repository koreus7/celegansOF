#pragma once
#include "catch.h"
#include "../serializable.h"
#include "../serializable_library.h"

class MockSerializable : public ISerializable
{
public:
    int childInt = 24;
    int mockInt = 12;

    void serialize(ofxJSONElement &root) const override
    {
        Json::Value childObject = Json::Value(Json::objectValue);

        childObject["childInt"] = childInt;

        root["mockInt"] = mockInt;
        root["child"] = childObject;

    }

    void deserialize(const ofxJSONElement &root) override
    {
        mockInt = root["mockInt"].asInt();
        childInt = root["child"]["childInt"].asInt();
    }

    std::string getUniqueId() const override
    {
        return "MockSerializable";
    }

};

class MockSerializableDifferentId : public ISerializable
{
public:
    int intValue = 0;
    float floatValue = 5.2;

    void serialize(ofxJSONElement &root) const override
    {
        root["intValue"] = intValue;
        root["floatValue"] = floatValue;
    }

    void deserialize(const ofxJSONElement &root) override
    {
        intValue = root["intValue"].asInt();
        floatValue = root["floatValue"].asFloat();
    }

    std::string getUniqueId() const override
    {
        return "MockSerializableDifferentId";
    }
};

TEST_CASE( "Serializes and Deserializes", "[serializable]" )
{

    SECTION("Serializes to ofxJSONElement")
    {

        MockSerializable mock;

        ofxJSONElement root;

        mock.serialize(root);

        REQUIRE( root["mockInt"].asInt() == 12 );
        REQUIRE( root["child"]["childInt"].asInt() == 24 );

    }

    SECTION("Deserializes from ofxJSONElement")
    {
        MockSerializable mock;

        mock.childInt = 500;
        mock.mockInt = 800;

        ofxJSONElement root;

        mock.serialize(root);

        MockSerializable mock2;

        mock2.deserialize(root);

        REQUIRE(mock2.childInt == 500);
        REQUIRE(mock2.mockInt == 800);
    }

    SECTION("Deserializes from string")
    {

        MockSerializable mock;

        std::string stringData = "{   \"child\" : {  \"childInt\" : 801   }, \"mockInt\" : 501 }";

        mock.deserializeJSONString(stringData);

        REQUIRE( mock.mockInt == 501 );
        REQUIRE( mock.childInt == 801 );

    }

    SECTION("Completes string serialization and deserialization round trip")
    {
        MockSerializable mock;

        mock.mockInt = 888;
        mock.childInt = 777;

        std::string stringSerialization = mock.serializeToJSONString();

        MockSerializable mock2;

        mock2.deserializeJSONString(stringSerialization);

        REQUIRE( mock2.mockInt == 888 );
        REQUIRE( mock2.childInt == 777 );

    }
}

TEST_CASE("Serializable library", "[serializable]")
{
    SECTION("Serializes objects to string")
    {
        SerializableLibrary library;

        MockSerializable object1;

        MockSerializableDifferentId object2;

        library.registerObject(object1);
        library.registerObject(object2);

        std::string serializedString = library.serializeAllToString();

        REQUIRE( serializedString == "{\n   \"MockSerializable\" : {\n      \"child\" : {\n         \"childInt\" : 24\n      },\n      \"mockInt\" : 12\n   },\n   \"MockSerializableDifferentId\" : {\n      \"floatValue\" : 5.199999809265137,\n      \"intValue\" : 0\n   }\n}\n" );

    }

    SECTION("Deserializes objects it serializes to string (round trip)")
    {
        SerializableLibrary library;

        MockSerializable object1;
        object1.mockInt = 1;
        object1.childInt = 2;

        MockSerializableDifferentId object2;
        object2.intValue = 3;
        object2.floatValue = 4.5;

        library.registerObject(object1);
        library.registerObject(object2);

        std::string serializedString = library.serializeAllToString();

        SerializableLibrary librarySecondInstance;

        MockSerializable object1SecondInstance;
        MockSerializableDifferentId object2SecondInstance;

        library.deserializeFromJSONString(serializedString);

        library.registerObject(object1SecondInstance);
        library.registerObject(object2SecondInstance);

        REQUIRE( object1SecondInstance.mockInt == 1 );
        REQUIRE( object1SecondInstance.childInt == 2 );
        REQUIRE( object2SecondInstance.intValue == 3 );
        REQUIRE( object2SecondInstance.floatValue < 4.50001 );
        REQUIRE( object2SecondInstance.floatValue > 4.49999 );

    }

}


