**DUE DATE: mm/dd/yyyy**

## SPECIFICATIONS
**As a** back-end developer **I want to** provide Data Engine with a Tag controller **so that** client applications can perform CRUD operations on Tag entities.

### Acceptance Criteria
- Add route `/Tag`
- Provide controller with handlers to
    - Retrieve all entities
      - [ ] Handler for `GET /Tag` to retrieve all
      - [ ] Must return `JSON` with all entities in the database
      - [ ] Each entity comes along with its id
    - Retrieve entity
      - [ ] `GET /Tag/{id}` to get specific entity
      - [ ] Returns a JSON representation of the entity
    - Create entity
      - [ ] `POST /Tag` to create entity
      - [ ] Expects entity's `JSON` representation
      - [ ] Returns error on invalid data
      - [ ] Returns OK and new entity representation
    - Modify entity
      - [ ] `PUT /Tag/{id}` to edit specific entity
      - [ ] Expects entity's `JSON` representation
      - [ ] Returns error on invalid data
      - [ ] Returns OK and updated entity representation
    - [ ] `DELETE /Tag/{id}` to delete specific entity
      - [ ] Returns error if id does not exist
      - [ ] Returns OK if completely removed
- Schema changes
    - [ ] The `label` column of the `Tag` table should be enforced unique by the database.

#### Analysis



#### Development
Development requirements.
#### Consider the following sources
- Use bulles to list possible research sources*.
[*] use proper markdown using titled links: `[Title](url)`

---

## ANALYSIS AND DEVELOPMENT

This portion of the feature request is to be filled by the _assignee_ as part of the pre-development and development process. 

The developer should provide exhaustive documentation of the analysis (the planning to realize the story) and the development (how it was done). The items of the issuer analysis must be addressed and the acceptance criteria met for closing.

This should happen as part of the transition between _Issued_ and _Developing_ columns of the project board.  

---

## TESTS
|Given|When|Then|
|---|---|---|
|A defined state|An action is performed|An specific result is expected|
