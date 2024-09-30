<h3 align="center">Ektishaf Unreal API</h3>

  <p align="center">
     <img src="https://content.pstmn.io/9a4265d8-6283-42c1-b769-b043d3e9af7d/SWNvbiA1MDB4NTAwLnBuZw==" width="64" height="64"><br/>
    This API allows Unreal developers to register new in-game wallets (or use an existing wallet), enables them to communicate seamlessly with EVM compatible blockchains and makes it fun to implement NFTs in Unreal Engine<br/>
  </p>
</div>

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
<br/>
Before we jump into installing the Unreal Plugin, feel free to check our website if you want to implement using REST API.<br/>
_If you have questions, feel free to email us at mail@ektishaf.com_ or check our [official website](https://ektishaf.com) and [documentation](https://ektishaf.com/documentation).
<br/><br/>

> [!IMPORTANT]
> * We never store any private key or password in any database
> * We never ask for any private information from our customers

> [!TIP]
> * Private keys and passwords should never be disclosed anywhere at all other than wherever required by the API.
> * We further suggest to use cryptographic services if you have planned to store any critical response data in a secured PlayerPrefs.

### Welcome! 👋
Welcome to Ektishaf Unreal API!.<br/>
Please read below to install the API plugin and have an understanding of the core functionalities to start right away.
<br/><br/>

### Prerequisites
Unreal Version: 5+
<br/><br/>

### 🔧 Install Package 
```
1. Clone the repo https://github.com/ektishafgithub/ektishaf-api-unreal.git.
2. Copy Ektishaf and EktishafEditor folders and paste them in your Plugins folder of your Unreal's project.
3. Regenerate Visual Studio project files.
```
That's it the plugin will be added to Unreal Engine.<br/>
If you are using a single .uplugin, please make sure to add Ektishaf and EktishafEditor to the modules array. All core functions can then be executed through **UEktishafSubsystem.h** file.
<br/><br/>

### 🔨 Unreal Documentation
This is an Unreal documentation explaining each of the core functions in **UEktishafSubsystem.h** file.
<br/><br/>
These are the delegates used as responses when a request is made. So for C++ **FEktishafOnResponseFast** is used and for Blueprint **FEktishafOnResponse** is used.
```cpp
DECLARE_DELEGATE_FourParams(FEktishafOnResponseFast, bool Success, const TArray<uint8> Bytes, const FString Content, TSharedPtr<FJsonObject> JsonObject);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FEktishafOnResponse, bool, success, const FString, content);
```
<br/>

All Blueprint functions are prefixed with **K2_** in **UEktishafSubsystem.h** file. In this documentation we have considered explaining C++ functions.

#### Host
Checks if the host is alive and returns a status code (200) with a welcome message showing readiness for blockchain interaction.

```cpp
  void UEktishafSubsystem::Host(const FEktishafOnResponseFast& Callback)
```

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Callback` | `FEktishafOnResponseFast` | **Response**. A response returned with Success and Content values respectively |

| Response | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `string` | `FString` | **Returns**. A string containing a welcome message. |

#### Register
Creates a new in-game wallet for the user based on the specified unique complex password.

```cpp
  void UEktishafSubsystem::Register(const FString Password, const FEktishafOnResponseFast& Callback)
```

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Password` | `FString` | **Required**. A new unique complex password to be specified for the wallet |
| `Callback` | `FEktishafOnResponseFast` | **Response**. A response returned with Success, Content and JsonObject values respectively |

| Response | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Json Object` | `FString` | **Returns**. A json object string result containing a wallet address and it's ticket |


#### Login
Logins with an existing wallet based on the ticket and the password.

```cpp
  void UEktishafSubsystem::Login(const FString Ticket, const FString Password, const FEktishafOnResponseFast& Callback)
```

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Ticket` | `FString` | **Required**. A valid ticket that was issued by the API |
| `Password` | `FString` | **Required**. The unique complex password of the wallet |
| `Callback` | `FEktishafOnResponseFast` | **Response**. A response returned with Success, Content and JsonObject values respectively |

| Response | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Json Object` | `FString` | **Returns**. A json object string result containing a wallet address and it's ticket |

#### External
Uses an external wallet to login based on the specified private key.

```cpp
  void UEktishafSubsystem::External(const FString PrivateKey, const FString Password, const FEktishafOnResponseFast& Callback)
```

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `PrivateKey` | `FString` | **Required**. Private key of the external wallet to be used in game |
| `Password` | `FString` | **Required**. A new unique complex password to be specified for the external wallet |
| `Callback` | `FEktishafOnResponseFast` | **Response**. A response returned with Success, Content and JsonObject values respectively |

| Response | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Json Object` | `FString` | **Returns**. A json object string result containing a wallet address and it's ticket |

#### Sign
Signs a message with the logged in wallet.

```cpp
  void UEktishafSubsystem::Sign(const FString Ticket, const FString Message, const FEktishafOnResponseFast& Callback)
```

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Ticket` | `FString` | **Required**. A valid ticket that was issued by the API |
| `Message` | `FString` | **Required**. Private key of the external wallet to be used in game |
| `Callback` | `FEktishafOnResponseFast` | **Response**. A response returned with Success, Content and JsonObject values respectively |

| Response | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Json Object` | `FString` | **Returns**. A json object string data containing the message and it's signature |

#### Verify
Verifies the signed message.

```cpp
  void UEktishafSubsystem::Verify(const FString Address, const FString Message, const FString Signature, const FEktishafOnResponseFast& Callback)
```

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Address` | `FString` | **Required**. The wallet address that has signed the message |
| `Message` | `FString` | **Required**. The message that was signed |
| `Signature` | `FString` | **Required**. The signature of the signed message |
| `Callback` | `FEktishafOnResponseFast` | **Response**. A response returned with Success, Content and JsonObject values respectively |

| Response | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Json Object` | `FString` | **Returns**. A json object string data containing a verification boolean, where true means verified |

#### Balance
Gets the eth balance on the wallet.

```cpp
  void UEktishafSubsystem::Balance(const FString Rpc, const FString Ticket, const FEktishafOnResponseFast& Callback)
```

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Rpc` | `FString` | **Required**. The wallet address that has signed the message |
| `Callback` | `FEktishafOnResponseFast` | **Response**. A response returned with Success, Content and JsonObject values respectively |
| `Ticket` | `FString` | **Required**. A valid ticket that was issued by the API |

| Response | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Json Object` | `FString` | **Returns**. A json object string data containing the balance |

#### ABI
Gets a Human-Readable ABI based on the specified Contract ABI.

```cpp
  void UEktishafSubsystem::ABI(const FString Abi, const bool Minimal, const FEktishafOnResponseFast& Callback)
```

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Abi` | `FString` | **Required**. The wallet address that has signed the message |
| `Minimal` | `FString` | **Required**. The wallet address that has signed the message |
| `Callback` | `FEktishafOnResponseFast` | **Response**. A response returned with Success, Content and JsonObject values respectively |

| Response | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Json Object` | `FString` | **Returns**. A json object string data containing the Human-Readable ABI |

#### Read
Executes a read-only function in a smart contract and returns any data if applicable.

```cpp
  void UEktishafSubsystem::Read(const FString Rpc, const FString Ticket, const FString Contract, const FString Abi, const FString Function, const TArray<TSharedPtr<FJsonValue>> Args, const FEktishafOnResponseFast& Callback)
```

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Rpc` | `FString` | **Required**. The rpc to be used for the blockchain interaction |
| `Contract` | `FString` | **Required**. The address of the contract |
| `Abi` | `FString` | **Required**. The abi of the contract or the Human-Readable ABI of the function |
| `Function` | `FString` | **Required**. The name of the function in the contract |
| `Callback` | `FEktishafOnResponseFast` | **Response**. A response returned with Success, Content and JsonObject values respectively |
| `Ticket` | `FString` | **Required**. A valid ticket that was issued by the API |
| `Args` | `FString` | **Required**. The arguments required by the function in the contract |

| Response | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Json Object` | `FString` | **Returns**. A json object string data containing the result of the function executed. |

#### Write
Executes a state-changing function in a smart contract and returns any transaction data if applicable.

```cpp
  void UEktishafSubsystem::Write(const FString Rpc, const FString Ticket, const FString Contract, const FString Abi, const FString Function, const TArray<TSharedPtr<FJsonValue>> Args, const FEktishafOnResponseFast& Callback)
```

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Rpc` | `FString` | **Required**. The rpc to be used for the blockchain interaction |
| `Contract` | `FString` | **Required**. The address of the contract |
| `Abi` | `FString` | **Required**. The abi of the contract or the Human-Readable ABI of the function |
| `Function` | `FString` | **Required**. The name of the function in the contract |
| `Callback` | `FEktishafOnResponseFast` | **Response**. A response returned with Success, Content and JsonObject values respectively |
| `Ticket` | `FString` | **Required**. A valid ticket that was issued by the API |
| `Args` | `FString` | **Required**. The arguments required by the function in the contract |

| Response | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Json Object` | `FString` | **Returns**. A json object string data containing the result of the function executed. |

## Authors

- [@ektishafgithub](https://www.github.com/ektishafgithub)

## 🔗 Links
[![linkedin](https://img.shields.io/badge/linkedin-0A66C2?style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/ektishaf)

## Support

For support, email us at mail@ektishaf.com
