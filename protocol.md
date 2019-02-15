# QUIZNET SPECIFICATIONS #

---

## REQUESTS ##

### General request especification ###

All *Requests* must adherer to the following syntactic specification

```text
type( length\n
body\n)?
```

|Field|Syntax|Semantics|Optional|
|---|---|---|---|
|`type`|Only **one** character from a-z|Indicates the type of request|No, all requests must have a `type` field|
|`length`|A decimal nonnegative integer|Describes the length of the request's `body` in bytes|Yes, an empty request is not expected to describe a length|
|`body`|Any number of ASCII characters|Request's body and payload|Yes, empty requests are allowd|

More precisely,

```EBNF
request = type , (' ' , length\n , body\n)?
        | type , id
        ;
type    = a-z
        ;
length  = [0-9]+
        ;
body    = ? any sequence of chars ?
        ;
```

If the server knows how to handle the type of request, the request is properly formed, and is processed successfully, the server will respond according to specifications. Otherwise, an error response will be returned. If the error is due to an unrecognized type of request, the error type will be `UNKREQ`.

### Types of requests ###

#### POST ####

Upload a question to the server

```plain-text
p length\n
body\n
```

This type of request must include a `length` field and a `body` field containing,

```text
tags\n
question-title\n.\n
choices.\n
(correct-answer)\n
```

1. `tags` field with zero or more comma separated tags.
2. `question-title` with a string of alphanumeric and white space characters terminated by the sequence `\n.\n`.
3. Any number of `choices` of the form `(choice-character) choice-text\n.\n` followed by an additional `\n.\n` that ends the section.
4. The correct answer `correct-answer` followed by an end of line.

Please observe that the server will enforce the following rules

1. A question must contain at least 2 choices.
2. Every choice must be of the format `(choice-character) choice-text\n.\n`.
3. The `choice-character` value is a unique a-z character, the choices must be in alphabetic order, and the first choice must be `a`.
4. The `correct-answer` provided must exist in the set of choices.

Expect an error response if any of this rules is violated. The error is likely to indicate in the extra field, what kind of violation took place.

#### DELETE ####

Request a deletion using type `d` and indicating the question's identifier in the body of the message.

```plain-text
d length\n
question-id\n
```

Expect an `NOTFND` error response if question doest not exist, or a `INVQID` if the the question id is not a numeric value.

#### GET ####

Request a question using type `g` and indicating the question's identifier in the body of the message.

```plain-text
g length\n
question-id\n
```

Expect an `NOTFND` error response if question doest not exist, or a `INVQID` if the the question id is not a numeric value.

#### GETR ####

Request a random question.

```plain-text
r \n
```

Expect an `EMPTYQ` error response if the server has an empty quiz book.

#### CHECK ####

Check if a choice is the valid response to a question identified by its question id.

```plain-text
c length\n
question-id\n
choice\n
```

Expect a `NOTFND` error if question does not exist or a `CHNFND` if the question does not contain the provided choice.

#### KILL ####

Kill server process.

```plain-text
k\n
```

---

## RESPONSES ##

All _responses_ will come in two types, OK responses indicating a successful request and ERROR responses along with information on the failure.

### Types of responses ###

#### OK ####

```plain-text
o length\n
body\n
```

The content fo the body of the response will depend on the particular request that unleashed the response.

#### ERROR ####

Error responses along with error codes and extra information.

```plain-text
e\n
[number]\n
[symbol]\n
[message]\n.\n
[extra]\n.\n
```

|no|symbol|message|extra|
|---|---|---|---|
|0|`UNKERR`|Unkown error|Exception message, if any|
|1|`NOTFND`|Question not found|`[question-id]` provided|
|2|`MALQUE`|Malformed question body|Exception message, if any|
|3|`CHNFND`|Choice does not exist in question|`[choice]` and `[question-id]`|
|4|`UNKREQ`|Server does not know how to handle request type|Received request type|
|5|`INVQID`|Invalid format for question id provided|Provided id|
|6|`EMPTYQ`|Empty quiz book|None|
|7|`REQTSZ`|Request type length must be one character|Type provided|
