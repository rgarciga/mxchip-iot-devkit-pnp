﻿/************************************************************************************************
 * This code was automatically generated by Digital Twin Code Generator tool 0.6.0.
 * Changes to this file may cause incorrect behavior and will be lost if the code is regenerated.
 *
 * Generated Date: Tuesday, August 13, 2019
 ***********************************************************************************************/

#include "screen_interface.h"

#define MAX_MASSAGE_SIZE 256

//
//  Callback function declarations and DigitalTwin command names for this interface.
//

static void ScreenInterface_Command_EchoCallback(const DIGITALTWIN_CLIENT_COMMAND_REQUEST* commandRequest, DIGITALTWIN_CLIENT_COMMAND_RESPONSE* commandResponse, void* userInterfaceContext);

static void ScreenInterface_Command_CountdownCallback(const DIGITALTWIN_CLIENT_COMMAND_REQUEST* commandRequest, DIGITALTWIN_CLIENT_COMMAND_RESPONSE* commandResponse, void* userInterfaceContext);

//
// Application state associated with this interface.
// It contains the DIGITALTWIN_INTERFACE_CLIENT_HANDLE used for responses in callbacks along with properties set
// and representations of the property update and command callbacks invoked on given interface
//
typedef struct SCREEN_INTERFACE_STATE_TAG
{
    DIGITALTWIN_INTERFACE_CLIENT_HANDLE interfaceClientHandle;

} SCREEN_INTERFACE_STATE;

static SCREEN_INTERFACE_STATE appState;

// Callback function to process the command "echo".
void ScreenInterface_Command_EchoCallback(const DIGITALTWIN_CLIENT_COMMAND_REQUEST* commandRequest, DIGITALTWIN_CLIENT_COMMAND_RESPONSE* commandResponse, void* userInterfaceContext)
{
    SCREEN_INTERFACE_STATE* interfaceState = (SCREEN_INTERFACE_STATE*)userInterfaceContext;
    LogInfo("SCREEN_INTERFACE: echo command invoked.");
    LogInfo("SCREEN_INTERFACE: echo request payload=<%.*s>, context=<%p>", (int)commandRequest->requestDataLen, commandRequest->requestData, interfaceState);

    // Get payload string
    char* requestData = (char*)malloc(commandRequest->requestDataLen + 1);
    if (requestData != NULL)
    {
        strncpy(requestData, (char*)commandRequest->requestData, commandRequest->requestDataLen);
        requestData[commandRequest->requestDataLen] = 0;
    }
    else
    {
        LogError("Failed to allocate memory for request payload");
    }

    // Parse request parameters from request payload string
    JSON_Value *jsonValue = NULL;
    jsonValue = json_parse_string((const char*)requestData);

    char* text = NULL;
    mallocAndStrcpy_s(&text, (char*)json_value_get_string(jsonValue));

    // Invoke device command here
    unsigned int statusCode = 200;

    char responsePayload[MAX_MASSAGE_SIZE];

    SCREEN_ECHO_echoResponse response;

    DIGITALTWIN_COMMAND_RESULT commandResult = Screen_Command_Echo(text, &response, &statusCode);

    // Serialize command response payload

    if (commandResult == DIGITALTWIN_COMMAND_OK &&
        Screen_SerializeEchoCommandResponse(&response, responsePayload, MAX_MASSAGE_SIZE))
    {
        DigitalTwinClientHelper_SetCommandResponse(commandResponse, responsePayload, statusCode);

    }
    else
    {
        LogError("Failed to execute echo command");
        char* errorResponse = "\"Failed to execute echo command\"";
        DigitalTwinClientHelper_SetCommandResponse(commandResponse, errorResponse, statusCode);
    }

    if (jsonValue)
    {
        json_value_free(jsonValue);
    }

    if (text != NULL)
    {
        free(text);
    }

}

// Callback function to process the command "countdown".
void ScreenInterface_Command_CountdownCallback(const DIGITALTWIN_CLIENT_COMMAND_REQUEST* commandRequest, DIGITALTWIN_CLIENT_COMMAND_RESPONSE* commandResponse, void* userInterfaceContext)
{
    SCREEN_INTERFACE_STATE* interfaceState = (SCREEN_INTERFACE_STATE*)userInterfaceContext;
    LogInfo("SCREEN_INTERFACE: countdown command invoked.");
    LogInfo("SCREEN_INTERFACE: countdown request payload=<%.*s>, context=<%p>", (int)commandRequest->requestDataLen, commandRequest->requestData, interfaceState);

    // Get payload string
    char* requestData = (char*)malloc(commandRequest->requestDataLen + 1);
    if (requestData != NULL)
    {
        strncpy(requestData, (char*)commandRequest->requestData, commandRequest->requestDataLen);
        requestData[commandRequest->requestDataLen] = 0;
    }
    else
    {
        LogError("Failed to allocate memory for request payload");
    }

    // Parse request parameters from request payload string
    JSON_Value *jsonValue = NULL;
    jsonValue = json_parse_string((const char*)requestData);

    int number = (int)json_value_get_number(jsonValue);

    // Invoke device command here
    unsigned int statusCode = 200;

    DIGITALTWIN_COMMAND_RESULT commandResult = Screen_Command_Countdown(number, &statusCode);

    if (commandResult == DIGITALTWIN_COMMAND_OK)
    {
        DigitalTwinClientHelper_SetCommandResponse(commandResponse, NULL, statusCode);

    }
    else
    {
        LogError("Failed to execute countdown command");
        char* errorResponse = "\"Failed to execute countdown command\"";
        DigitalTwinClientHelper_SetCommandResponse(commandResponse, errorResponse, statusCode);
    }

    if (jsonValue)
    {
        json_value_free(jsonValue);
    }

}

// DigitalTwinSample_ProcessCommandUpdate receives commands from the server.  This implementation acts as a simple dispatcher
// to the functions to perform the actual processing.
void ScreenInterface_ProcessCommandUpdate(const DIGITALTWIN_CLIENT_COMMAND_REQUEST* dtCommandRequest, DIGITALTWIN_CLIENT_COMMAND_RESPONSE* dtCommandResponse, void* userInterfaceContext)
{
    if (strcmp(dtCommandRequest->commandName, ScreenInterface_EchoCommand) == 0)
    {
        ScreenInterface_Command_EchoCallback(dtCommandRequest, dtCommandResponse, userInterfaceContext);
        return;
    }

    if (strcmp(dtCommandRequest->commandName, ScreenInterface_CountdownCommand) == 0)
    {
        ScreenInterface_Command_CountdownCallback(dtCommandRequest, dtCommandResponse, userInterfaceContext);
        return;
    }

    // If the command is not implemented by this interface, by convention we return a 501 error to server.
    LogError("SCREEN_INTERFACE: Command name <%s> is not associated with this interface", dtCommandRequest->commandName);
    (void)DigitalTwinClientHelper_SetCommandResponse(dtCommandResponse, "\"Requested command not implemented on this interface\"", 501);
}

// ScreenInterface_InterfaceRegisteredCallback is invoked when this interface
// is successfully or unsuccessfully registered with the service, and also when the interface is deleted.
static void ScreenInterface_InterfaceRegisteredCallback(DIGITALTWIN_CLIENT_RESULT dtInterfaceStatus, void* userInterfaceContext)
{
    LogInfo("ScreenInterface_InterfaceRegisteredCallback with status=<%s>, userContext=<%p>", MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, dtInterfaceStatus), userInterfaceContext);
    if (dtInterfaceStatus == DIGITALTWIN_CLIENT_OK)
    {
        // Once the interface is registered, send our reported properties to the service.  
        // It *IS* safe to invoke most DigitalTwin API calls from a callback thread like this, though it 
        // is NOT safe to create/destroy/register interfaces now.
        LogInfo("SCREEN_INTERFACE: Interface successfully registered.");
    }
    else if (dtInterfaceStatus == DIGITALTWIN_CLIENT_ERROR_INTERFACE_UNREGISTERING)
    {
        // Once an interface is marked as unregistered, it cannot be used for any DigitalTwin SDK calls.
        LogInfo("SCREEN_INTERFACE: Interface received unregistering callback.");
    }
    else
    {
        LogError("SCREEN_INTERFACE: Interface received failed, status=<%s>.", MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, dtInterfaceStatus));
    }
}

//
// Create DigitalTwin interface client handle
//
DIGITALTWIN_INTERFACE_CLIENT_HANDLE ScreenInterface_Create()
{
    DIGITALTWIN_INTERFACE_CLIENT_HANDLE interfaceHandle;
    DIGITALTWIN_CLIENT_RESULT result;

    memset(&appState, 0, sizeof(SCREEN_INTERFACE_STATE));

    if ((result = DigitalTwin_InterfaceClient_Create(ScreenInterfaceId,  ScreenInterfaceInstanceName, ScreenInterface_InterfaceRegisteredCallback, (void*)&appState, &interfaceHandle)) != DIGITALTWIN_CLIENT_OK)
    {
        LogError("SCREEN_INTERFACE: Unable to allocate interface client handle for interfaceId=<%s>, interfaceInstanceName=<%s>, error=<%s>", ScreenInterfaceId, ScreenInterfaceInstanceName, MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, result));
        interfaceHandle = NULL;
    }

    else if ((result = DigitalTwin_InterfaceClient_SetCommandsCallback(interfaceHandle, ScreenInterface_ProcessCommandUpdate)) != DIGITALTWIN_CLIENT_OK)
    {
        LogError("SCREEN_INTERFACE: DigitalTwin_InterfaceClient_SetCommandsCallbacks failed. error=<%s>", MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, result));
        ScreenInterface_Close(interfaceHandle);
        interfaceHandle = NULL;
    }

    else
    {
        LogInfo("SCREEN_INTERFACE: Created DIGITALTWIN_INTERFACE_CLIENT_HANDLE successfully for interfaceId=<%s>, interfaceInstanceName=<%s>, handle=<%p>", ScreenInterfaceId, ScreenInterfaceInstanceName, interfaceHandle);
        appState.interfaceClientHandle = interfaceHandle;
    }

    return interfaceHandle;
}

void ScreenInterface_Close(DIGITALTWIN_INTERFACE_CLIENT_HANDLE digitalTwinInterfaceClientHandle)
{
    // On shutdown, in general the first call made should be to DigitalTwin_InterfaceClient_Destroy.
    // This will block if there are any active callbacks in this interface, and then
    // mark the underlying handle such that no future callbacks shall come to it.
    DigitalTwin_InterfaceClient_Destroy(digitalTwinInterfaceClientHandle);

    // After DigitalTwin_InterfaceClient_Destroy returns, it is safe to assume
    // no more callbacks shall arrive for this interface and it is OK to free
    // resources callbacks otherwise may have needed.

}
